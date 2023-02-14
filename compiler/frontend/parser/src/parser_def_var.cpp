// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_def_var.hpp"

#include <array>

#include "parser_expression.hpp"

auto var_def_error_handler_(ParserContext& ctx) -> ParserState;
auto var_def_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState;

auto var_def_exit_handler_(ParserContext& ctx) -> ParserState;
auto var_def_end_handler_(ParserContext& ctx) -> ParserState;

auto var_def_assign_end_handler_(ParserContext& ctx) -> ParserState;
auto var_def_assign_handler_(ParserContext& ctx) -> ParserState;

auto var_def_assign_start_handler_(ParserContext& ctx) -> ParserState;

auto var_def_type_handler_(ParserContext& ctx) -> ParserState;
auto var_def_type_start_handler_(ParserContext& ctx) -> ParserState;

auto var_def_name_handler_(ParserContext& ctx) -> ParserState;

auto var_def_mut_handler_(ParserContext& ctx) -> ParserState;

auto var_def_start_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState var_def_error_state = ParserState{var_def_error_handler_};
constexpr ParserState var_def_unexpected_end_error_state =
    ParserState{var_def_unexpected_end_error_handler_};

constexpr ParserState var_def_exit_state         = ParserState{var_def_exit_handler_};
constexpr ParserState var_def_end_state          = ParserState{var_def_end_handler_};
constexpr ParserState var_def_assign_end_state   = ParserState{var_def_assign_end_handler_};
constexpr ParserState var_def_assign_state       = ParserState{var_def_assign_handler_};
constexpr ParserState var_def_assign_start_state = ParserState{var_def_assign_start_handler_};
constexpr ParserState var_def_type_state         = ParserState{var_def_type_handler_};

constexpr ParserState var_def_type_start_state   = ParserState{var_def_type_start_handler_};
constexpr ParserState var_def_name_state         = ParserState{var_def_name_handler_};
constexpr ParserState var_def_mut_state          = ParserState{var_def_mut_handler_};
constexpr ParserState var_def_start_state        = ParserState{var_def_start_handler_};

auto var_def_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto var_def_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

// var x : i32 = 0 [ ; ]
auto var_def_exit_handler_(ParserContext& ctx) -> ParserState { return ctx.move_next_stack(); }

auto var_def_end_handler_(ParserContext& ctx) -> ParserState {
  if (!is_semicolon(ctx.current())) {
    return var_def_error_state;
  }
  return var_def_exit_state;
}

// var x : i32 = [ 0 ];
auto var_def_assign_end_handler_(ParserContext& ctx) -> ParserState {
  auto assignment = ctx.pop_syntax_node<BaseExpression>();
  ctx.get_syntax_node<VariableDefinition>().set_assignment(std::move(assignment));
  return var_def_end_state;
}

// var x : i32 = [ 0 ];
auto var_def_assign_handler_(ParserContext& ctx) -> ParserState {
  ctx.push_states(var_def_assign_end_state, var_def_unexpected_end_error_state);
  return expr_start_state;
}

// var x : i32 [ = ] 0;
auto var_def_assign_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_equals(ctx.current()));
  return ctx.move_next_state(var_def_assign_state, var_def_unexpected_end_error_state);
}

// var x : [ i32 ] = 0;
auto var_def_type_handler_(ParserContext& ctx) -> ParserState {
  assert(is_identifier(ctx.current()));

  ctx.get_syntax_node<VariableDefinition>().set_type_name(ctx.current().value());

  return ctx.move_next_state(
      is_equals, var_def_assign_start_state, var_def_end_state, var_def_unexpected_end_error_state);
}

// var x [ : ] i32 = 0;
auto var_def_type_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_colon(ctx.current()));
  return ctx.move_next_state(
      is_identifier, var_def_type_state, var_def_error_state, var_def_unexpected_end_error_state);
}

// var [ x ] : i32 = 0;
auto var_def_name_handler_(ParserContext& ctx) -> ParserState {
  assert(is_identifier(ctx.current()));

  ctx.get_syntax_node<VariableDefinition>().set_name(ctx.current().value());

  constexpr auto conditions = std::array<ParserContext::RefMatchCondition, 2>{
      ParserContext::RefMatchCondition{is_colon,  var_def_type_start_state  },
      ParserContext::RefMatchCondition{is_equals, var_def_assign_start_state}
  };

  return ctx.move_next_state(var_def_error_state, var_def_unexpected_end_error_state, conditions);
}

auto var_def_mut_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_mut(ctx.current()));
  ctx.get_syntax_node<VariableDefinition>().set_mutable(true);

  return ctx.move_next_state(is_identifier, var_def_name_state, var_def_error_state, var_def_unexpected_end_error_state);
}

// [ var ] x : i32 = 0;
auto var_def_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_keyword_var(current));
  ctx.syntax_stack.emplace(std::make_unique<VariableDefinition>(ctx.current().pos()));

  static constexpr auto conditions = std::array<ParserMatchCondition, 2>{
      ParserMatchCondition{is_identifier,  var_def_name_state},
      ParserMatchCondition{is_keyword_mut, var_def_mut_state }
  };

  return ctx.move_next_state(var_def_error_state, var_def_unexpected_end_error_state, conditions);
}
