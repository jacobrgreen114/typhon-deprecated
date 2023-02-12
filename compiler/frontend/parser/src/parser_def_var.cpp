// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_def_var.hpp"

#include <array>

#include "parser_expression.hpp"

auto var_def_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

constexpr ParserState var_def_error_state = ParserState{var_def_error_handler_};

auto var_def_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

constexpr ParserState var_def_unexpected_end_error_state =
    ParserState{var_def_unexpected_end_error_handler_};

// var x : i32 = 0 [ ; ]
auto var_def_exit_handler_(ParserContext& ctx) -> ParserState { return ctx.move_next_stack(); }
constexpr ParserState var_def_exit_state = ParserState{var_def_exit_handler_};

auto var_def_end_handler_(ParserContext& ctx) -> ParserState {
  if (!is_semicolon(ctx.current())) {
    return var_def_error_state;
  }
  return var_def_exit_state;
}
constexpr ParserState var_def_end_state = ParserState{var_def_end_handler_};

// var x : i32 = [ 0 ];
auto var_def_assign_end_handler_(ParserContext& ctx) -> ParserState {
  auto assignment = ctx.pop_expr_node();
  auto* var_def   = ctx.get_var_def_node();
  var_def->set_assignment(std::move(assignment));
  return var_def_end_state;
}

constexpr ParserState var_def_assign_end_state = ParserState{var_def_assign_end_handler_};

// var x : i32 = [ 0 ];
auto var_def_assign_handler_(ParserContext& ctx) -> ParserState {
  ctx.push_states(var_def_assign_end_state, var_def_error_state);
  return expr_start_state;
}

constexpr ParserState var_def_assign_state = ParserState{var_def_assign_handler_};

// var x : i32 [ = ] 0;
auto var_def_assign_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_equals(ctx.current()));
  return ctx.move_next_state(var_def_assign_state, var_def_unexpected_end_error_state);
}

constexpr ParserState var_def_assign_start_state = ParserState{var_def_assign_start_handler_};

// var x : [ i32 ] = 0;
auto var_def_type_handler_(ParserContext& ctx) -> ParserState {
  assert(is_identifier(ctx.current()));

  auto* var_def  = ctx.get_var_def_node();
  auto type_name = ctx.current().value();
  var_def->set_type_name(type_name);

  return ctx.move_next_state(
      is_equals, var_def_assign_start_state, var_def_end_state, var_def_unexpected_end_error_state);
}

constexpr ParserState var_def_type_state = ParserState{var_def_type_handler_};

// var x [ : ] i32 = 0;
auto var_def_type_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_colon(ctx.current()));
  return ctx.move_next_state(
      is_identifier, var_def_type_state, var_def_error_state, var_def_unexpected_end_error_state);
}

constexpr ParserState var_def_type_start_state = ParserState{var_def_type_start_handler_};

// var [ x ] : i32 = 0;
auto var_def_identifier_handler_(ParserContext& ctx) -> ParserState {
  assert(is_identifier(ctx.current()));

  auto* var_def        = ctx.get_var_def_node();
  auto identifier_name = ctx.current().value();
  var_def->set_name(identifier_name);

  constexpr auto conditions = std::array<ParserContext::RefMatchCondition, 2>{
      ParserContext::RefMatchCondition{is_colon,  var_def_type_start_state  },
      ParserContext::RefMatchCondition{is_equals, var_def_assign_start_state}
  };

  return ctx.move_next_state(var_def_error_state, var_def_unexpected_end_error_state, conditions);
}

constexpr ParserState var_def_identifier_state = ParserState{var_def_identifier_handler_};

// [ var ] x : i32 = 0;
auto var_def_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_keyword_var(current));
  ctx.syntax_stack.emplace(std::make_unique<VarDefinition>());
  return ctx.move_next_state(is_identifier,
                             var_def_identifier_state,
                             var_def_error_state,
                             var_def_unexpected_end_error_state);
}

constexpr ParserState var_def_start_state = ParserState{var_def_start_handler_};