// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_def_func.hpp"

#include "parser_statement.hpp"

auto func_def_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

constexpr ParserState func_def_error_state = ParserState{func_def_error_handler_};

auto func_def_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

constexpr ParserState func_def_unexpected_end_error_state =
    ParserState{func_def_unexpected_end_error_handler_};

auto do_func_def_body_end(ParserContext& ctx) -> void {
  auto block = ctx.pop_statement_block();
  auto* def  = ctx.get_func_def_node();
  def->set_body(block);
}

auto func_def_body_end_exit_handler_(ParserContext& ctx) -> ParserState {
  do_func_def_body_end(ctx);
  return ctx.pop_end_state();
}

constexpr ParserState func_def_body_end_exit_state = ParserState{func_def_body_end_exit_handler_};

auto func_def_body_end_handler_(ParserContext& ctx) -> ParserState {
  do_func_def_body_end(ctx);
  return ctx.pop_ret_state();
}

constexpr ParserState func_def_body_end_state = ParserState{func_def_body_end_handler_};

auto func_def_body_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_bracket_open(current));
  ctx.push_states(func_def_body_end_state, func_def_body_end_exit_state);
  return statement_block_start_state;
}

constexpr ParserState func_def_body_start_state = ParserState{func_def_body_start_handler_};

auto func_def_return_name_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_identifier(current));

  auto* func_def = ctx.get_func_def_node();
  func_def->set_return_type(current.value());

  return ctx.move_next_state(is_bracket_open,
                             func_def_body_start_state,
                             func_def_error_state,
                             func_def_unexpected_end_error_state);
}

constexpr ParserState func_def_return_name_state = ParserState{func_def_return_name_handler_};

auto func_def_return_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_arrow(current));
  return ctx.move_next_state(is_identifier,
                             func_def_return_name_state,
                             func_def_error_state,
                             func_def_unexpected_end_error_state);
}

constexpr ParserState func_def_return_start_state = ParserState{func_def_return_start_handler_};

auto func_def_param_block_end_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_paren_close(current));

  constexpr auto conditions = std::array<ParserContext::RefMatchCondition, 2>{
      ParserContext::RefMatchCondition{is_arrow,        func_def_return_start_state},
      ParserContext::RefMatchCondition{is_bracket_open, func_def_body_start_state  }
  };

  return ctx.move_next_state(func_def_error_state, func_def_unexpected_end_error_state, conditions);
  throw_not_implemented();
}

constexpr ParserState func_def_param_block_end_state =
    ParserState{func_def_param_block_end_handler_};

extern const ParserState func_def_param_start_state;

auto func_def_param_next_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_comma(current));
  return ctx.move_next_state(is_identifier,
                             func_def_param_start_state,
                             func_def_error_state,
                             func_def_unexpected_end_error_state);
}

constexpr ParserState func_def_param_next_state = ParserState{func_def_param_next_handler_};

auto func_def_param_end_handler_(ParserContext& ctx) -> ParserState {
  auto param     = ctx.pop_func_def_param();
  auto* func_def = ctx.get_func_def_node();
  func_def->push_parameter(param);

  static constexpr auto conditions = std::array<ParserContext::RefMatchCondition, 2>{
      ParserContext::RefMatchCondition{is_comma,       func_def_param_next_state     },
      ParserContext::RefMatchCondition{is_paren_close, func_def_param_block_end_state}
  };

  return ctx.move_next_state(func_def_error_state, func_def_unexpected_end_error_state, conditions);
}

constexpr ParserState func_def_param_end_state = ParserState{func_def_param_end_handler_};

auto func_def_param_type_name_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_identifier(current));

  auto* param_def = ctx.get_func_param_node();
  param_def->set_type_name(current.value());

  return func_def_param_end_state;
}

constexpr ParserState func_def_param_type_name_state =
    ParserState{func_def_param_type_name_handler_};

auto func_def_param_type_start_state_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_colon(current));
  return ctx.move_next_state(is_identifier,
                             func_def_param_type_name_state,
                             func_def_error_state,
                             func_def_unexpected_end_error_state);
}

constexpr ParserState func_def_param_type_start_state =
    ParserState{func_def_param_type_start_state_};

auto func_def_param_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_identifier(current));
  ctx.syntax_stack.push(std::make_shared<FuncParameter>(current.value()));

  static constexpr auto conditions = std::array<ParserContext::RefMatchCondition, 1>{
      ParserContext::RefMatchCondition{is_colon, func_def_param_type_start_state}
  };

  return ctx.move_next_state(
      func_def_param_end_state, func_def_unexpected_end_error_state, conditions);
}

constexpr ParserState func_def_param_start_state = ParserState{func_def_param_start_handler_};

// func main [ ( ] ) -> int { return 0; }
auto func_def_param_block_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_open(ctx.current()));

  static constexpr auto conditions = std::array<ParserContext::RefMatchCondition, 2>{
      ParserContext::RefMatchCondition{is_identifier,  func_def_param_start_state    },
      ParserContext::RefMatchCondition{is_paren_close, func_def_param_block_end_state}
  };
  return ctx.move_next_state(func_def_error_state, func_def_unexpected_end_error_state, conditions);
}

constexpr ParserState func_def_param_block_start_state =
    ParserState{func_def_param_block_start_handler_};

// func [ main ] ( ) -> int { return 0; }
auto func_def_identifier_handler_(ParserContext& ctx) -> ParserState {
  assert(is_identifier(ctx.current()));

  auto* func_def       = ctx.get_func_def_node();
  auto identifier_name = ctx.current().value();
  func_def->set_name(identifier_name);

  return ctx.move_next_state(is_paren_open,
                             func_def_param_block_start_state,
                             func_def_error_state,
                             func_def_unexpected_end_error_state);
}

constexpr ParserState func_def_identifier_state = ParserState{func_def_identifier_handler_};

// [ func ] main () -> int { return 0; }
auto func_def_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_keyword_func(current));
  ctx.syntax_stack.emplace(std::make_shared<FuncDefinition>());
  return ctx.move_next_state(is_identifier,
                             func_def_identifier_state,
                             func_def_error_state,
                             func_def_unexpected_end_error_state);
}

constexpr ParserState func_def_start_state = ParserState{func_def_start_handler_};
