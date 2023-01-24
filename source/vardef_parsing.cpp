// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "vardef_parsing.hpp"

#include <array>

#include "expression_parsing.hpp"

auto get_var_def_syntax_node(ParserContext& ctx) -> VarDefinition* {
  auto& p_syntax = ctx.syntax_stack.top();
  assert(p_syntax && p_syntax->kind() == SyntaxKind::DefVar);
  return static_cast<VarDefinition*>(p_syntax.get());
}

constexpr ParserState var_def_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

constexpr ParserState var_def_unexpected_end_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

// var x : i32 = 0 [ ; ]
constexpr ParserState var_def_exit_state = ParserState{
    [](ParserContext& ctx) -> ParserState { return ctx.move_next_stack(); }};

constexpr ParserState var_def_end_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      if (!is_semicolon(ctx.current())) {
        return var_def_error_state;
      }
      return var_def_exit_state;
    }};

// var x : i32 = [ 0 ];
constexpr ParserState var_def_assign_end_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto* var_def = get_var_def_syntax_node(ctx);
      auto& assignment = ctx.expression_stack.top();
      var_def->set_assignment(assignment);
      ctx.expression_stack.pop();
      assert(ctx.expression_stack.empty());
      return var_def_end_state;
    }};

// var x : i32 = [ 0 ];
constexpr ParserState var_def_assign_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      ctx.push_states(var_def_assign_end_state, var_def_error_state);
      return expression_start_state;
    }};

// var x : i32 [ = ] 0;
constexpr ParserState var_def_assign_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_equals(ctx.current()));
      return ctx.move_next_state(var_def_assign_state,
                                 var_def_unexpected_end_error_state);
    }};

// var x : [ i32 ] = 0;
constexpr ParserState var_def_type_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_identifier(ctx.current()));

      auto* var_def = get_var_def_syntax_node(ctx);
      auto type_name = ctx.current().value();
      var_def->set_type_name(type_name);

      return ctx.move_next_state(is_equals, var_def_assign_start_state,
                                 var_def_end_state,
                                 var_def_unexpected_end_error_state);
    }};

// var x [ : ] i32 = 0;
constexpr ParserState var_def_type_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_colon(ctx.current()));
      return ctx.move_next_state(is_identifier, var_def_type_state,
                                 var_def_error_state,
                                 var_def_unexpected_end_error_state);
    }};

// var [ x ] : i32 = 0;
constexpr ParserState var_def_identifier_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_identifier(ctx.current()));

      auto* var_def = get_var_def_syntax_node(ctx);
      auto identifier_name = ctx.current().value();
      var_def->set_name(identifier_name);

      constexpr auto conditions =
          std::array<ParserContext::RefMatchCondition, 2>{
              ParserContext::RefMatchCondition{is_colon,
                                               var_def_type_start_state},
              ParserContext::RefMatchCondition{is_equals,
                                               var_def_assign_start_state}};

      return ctx.move_next_state(
          var_def_error_state, var_def_unexpected_end_error_state, conditions);
    }};

// [ var ] x : i32 = 0;
constexpr ParserState var_def_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_keyword_var(current));
      ctx.syntax_stack.emplace(std::make_shared<VarDefinition>());
      return ctx.move_next_state(is_identifier, var_def_identifier_state,
                                 var_def_error_state,
                                 var_def_unexpected_end_error_state);
    }};