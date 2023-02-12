// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_expression.hpp"

// todo : implement postfix operators
// todo : convert period binary operator to access expression

/*
 * Error
 */

auto expr_error_handler_(ParserContext& ctx) -> ParserState;
auto expr_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState expr_error_state = ParserState{expr_error_handler_};
constexpr ParserState expr_unexpected_end_error_state =
    ParserState{expr_unexpected_end_error_handler_};

auto expr_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto expr_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

/*
 * Exit
 */

auto expr_exit_end_handler_(ParserContext& ctx) -> ParserState;
auto expr_exit_handler_(ParserContext& ctx) -> ParserState;
auto expr_end_handler_(ParserContext& ctx) -> ParserState;
auto expr_possible_end_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState expr_exit_end_state     = ParserState{expr_exit_end_handler_};
constexpr ParserState expr_exit_state         = ParserState{expr_exit_handler_};
constexpr ParserState expr_end_state          = ParserState{expr_end_handler_};
constexpr ParserState expr_possible_end_state = ParserState{expr_possible_end_handler_};

auto expr_exit_end_handler_(ParserContext& ctx) -> ParserState { return ctx.pop_end_state(); }

auto expr_exit_handler_(ParserContext& ctx) -> ParserState { return ctx.pop_ret_state(); }

auto expr_end_handler_(ParserContext& ctx) -> ParserState { return expr_exit_state; }

extern const ParserState expr_binary_state;
extern const ParserState expr_unknown_state;

auto expr_possible_end_handler_(ParserContext& ctx) -> ParserState {
  if (is_binary_operator(ctx.current())) {
    return expr_binary_state;
  }
  return expr_exit_state;
}

/*
 * Bool
 */

auto expr_bool_handler_(ParserContext& ctx) -> ParserState;

constexpr auto expr_bool_state = ParserState{expr_bool_handler_};

auto expr_bool_handler_(ParserContext& ctx) -> ParserState {
  auto kind = ctx.current().kind();
  assert(kind == LexicalKind::KeywordTrue || kind == LexicalKind::KeywordFalse);
  const auto value = kind == LexicalKind::KeywordTrue;
  ctx.syntax_stack.push(std::make_unique<BoolExpression>(value));
  return ctx.move_next_state(expr_possible_end_state, expr_exit_end_state);
}

/*
 * Number
 */

auto expr_number_handler_(ParserContext& ctx) -> ParserState;

constexpr auto expr_number_state = ParserState{expr_number_handler_};

auto expr_number_handler_(ParserContext& ctx) -> ParserState {
  const auto& value = ctx.current().value();
  ctx.syntax_stack.push(std::make_unique<NumberExpression>(value));
  return ctx.move_next_state(expr_possible_end_state, expr_exit_end_state);
}

/*
 * String
 */

auto expr_string_handler_(ParserContext& ctx) -> ParserState;

constexpr auto expr_string_state = ParserState{expr_string_handler_};

auto expr_string_handler_(ParserContext& ctx) -> ParserState {
  const auto& value = ctx.current().value();
  ctx.syntax_stack.push(std::make_unique<StringExpression>(value));
  return ctx.move_next_state(expr_possible_end_state, expr_exit_end_state);
}

/*
 * Identifier
 */

auto expr_call_end_handler_(ParserContext& ctx) -> ParserState;
auto expr_call_param_end_handler_(ParserContext& ctx) -> ParserState;
auto expr_call_param_start_handler_(ParserContext& ctx) -> ParserState;
auto expr_call_start_handler_(ParserContext& ctx) -> ParserState;

auto expr_access_end_handler_(ParserContext& ctx) -> ParserState;
auto expr_access_handler_(ParserContext& ctx) -> ParserState;

auto expr_ident_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState expr_call_end_state         = ParserState{expr_call_end_handler_};
constexpr ParserState expr_call_param_end_state   = ParserState{expr_call_param_end_handler_};
constexpr ParserState expr_call_param_start_state = ParserState{expr_call_param_start_handler_};
constexpr ParserState expr_call_start_state       = ParserState{expr_call_start_handler_};

constexpr ParserState expr_access_end_state       = ParserState{expr_access_end_handler_};
constexpr ParserState expr_access_state           = ParserState{expr_access_handler_};

constexpr ParserState expr_ident_state            = ParserState{expr_ident_handler_};

auto expr_call_end_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_close(ctx.current()));
  return ctx.move_next_state(expr_possible_end_state, expr_exit_end_state);
}

auto expr_call_param_end_handler_(ParserContext& ctx) -> ParserState {
  auto expr = ctx.pop_syntax_node<ExpressionNode>();
  auto call = ctx.get_syntax_node<CallExpression>();
  call->push_parameter(std::move(expr));

  auto& current = ctx.current();
  if (is_comma(current)) {
    return ctx.move_next_state(expr_call_param_start_state, expr_unexpected_end_error_state);
  } else if (is_paren_close(current)) {
    return expr_call_end_state;
  } else {
    return expr_error_state;
  }
}

auto expr_call_param_start_handler_(ParserContext& ctx) -> ParserState {
  ctx.push_states(expr_call_param_end_state, expr_unexpected_end_error_state);
  return expr_start_state;
}

auto expr_call_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_open(ctx.current()));
  const auto& value = ctx.token_stack.top().value();
  ctx.syntax_stack.push(std::make_unique<CallExpression>(value));
  ctx.token_stack.pop();

  return ctx.move_next_state(is_paren_close,
                             expr_call_end_state,
                             expr_call_param_start_state,
                             expr_unexpected_end_error_state);
}

auto do_expr_access(ParserContext& ctx) {
  const auto& value = ctx.token_stack.top().value();
  ctx.syntax_stack.push(std::make_unique<IdentifierExpression>(value));
  ctx.token_stack.pop();
}

auto expr_access_end_handler_(ParserContext& ctx) -> ParserState {
  do_expr_access(ctx);
  return expr_exit_end_state;
}

auto expr_access_handler_(ParserContext& ctx) -> ParserState {
  do_expr_access(ctx);
  return expr_possible_end_state;
}

// todo : implement function call
auto expr_ident_handler_(ParserContext& ctx) -> ParserState {
  ctx.push_current_token();
  return ctx.move_next_state(
      is_paren_open, expr_call_start_state, expr_access_state, expr_access_end_state);
}

/*
 * Binary
 */

auto expr_binary_end_exit_handler_(ParserContext& ctx) -> ParserState;
auto expr_binary_end_handler_(ParserContext& ctx) -> ParserState;
auto expr_binary_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState expr_binary_end_exit_state = ParserState{expr_binary_end_exit_handler_};
constexpr ParserState expr_binary_end_state      = ParserState{expr_binary_end_handler_};
constexpr ParserState expr_binary_state          = ParserState{expr_binary_handler_};

auto do_expr_binary_end(ParserContext& ctx) -> void {
  ctx.precedence_stack.pop();

  auto rhs = ctx.pop_expr_node();
  auto op  = ctx.pop_expr_binary_node();
  auto lhs = ctx.pop_expr_node();

  op->set_lhs(std::move(lhs));
  op->set_rhs(std::move(rhs));

  ctx.syntax_stack.emplace(std::move(op));
}

auto expr_binary_end_exit_handler_(ParserContext& ctx) -> ParserState {
  do_expr_binary_end(ctx);
  return ctx.pop_end_state();
}

auto expr_binary_end_handler_(ParserContext& ctx) -> ParserState {
  do_expr_binary_end(ctx);
  if (is_binary_operator(ctx.current())) {
    return expr_binary_state;
  }

  return ctx.pop_ret_state();
}

auto expr_binary_handler_(ParserContext& ctx) -> ParserState {
  const auto& current = ctx.current();
  assert(is_binary_operator(current));

  const auto op         = get_binary_op(current.kind());
  const auto precedence = get_precedence(op);

  if (!ctx.precedence_stack.empty() && ctx.precedence_stack.top() > precedence) {
    return ctx.pop_ret_state();
  }

  ctx.precedence_stack.emplace(precedence);
  ctx.syntax_stack.emplace(std::make_unique<BinaryExpression>(op));

  ctx.push_states(expr_binary_end_state, expr_binary_end_exit_state);
  return ctx.move_next_state(expr_unknown_state, expr_unexpected_end_error_state);
}

/*
 * Unary
 */

auto expr_unary_end_exit_handler_(ParserContext& ctx) -> ParserState;
auto expr_unary_end_handler_(ParserContext& ctx) -> ParserState;
auto expr_unary_handler_(ParserContext& ctx) -> ParserState;

constexpr auto expr_unary_end_exit_state = ParserState{expr_unary_end_exit_handler_};
constexpr auto expr_unary_end_state      = ParserState{expr_unary_end_handler_};
constexpr auto expr_unary_state          = ParserState{expr_unary_handler_};

void do_expr_unary_end(ParserContext& ctx) {
  ctx.precedence_stack.pop();
  auto expr   = ctx.pop_expr_node();
  auto* unary = ctx.get_expr_unary_node();
  unary->set_expr(std::move(expr));
}

auto expr_unary_end_exit_handler_(ParserContext& ctx) -> ParserState {
  do_expr_unary_end(ctx);
  return ctx.pop_end_state();
}

auto expr_unary_end_handler_(ParserContext& ctx) -> ParserState {
  do_expr_unary_end(ctx);
  // todo : redo unary and binary state entrances and exits
  if (is_binary_operator(ctx.current())) {
    return expr_binary_state;
  }
  return ctx.pop_ret_state();
}

auto expr_unary_handler_(ParserContext& ctx) -> ParserState {
  const auto& current = ctx.current();
  const auto op       = get_unary_pre_op(current.kind());

  const auto prec     = get_precedence(op);
  ctx.precedence_stack.push(prec);

  ctx.syntax_stack.push(std::make_unique<UnaryExpression>(op));
  ctx.push_states(expr_unary_end_state, expr_unary_end_exit_state);
  return ctx.move_next_state(expr_unknown_state, expr_unexpected_end_error_state);
}

/*
 * Start
 */

auto expr_unknown_handler_(ParserContext& ctx) -> ParserState;
auto expr_start_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState expr_unknown_state = ParserState{expr_unknown_handler_};
constexpr ParserState expr_start_state   = ParserState{expr_start_handler_};

auto expr_unknown_handler_(ParserContext& ctx) -> ParserState {
  auto kind = ctx.current().kind();
  if (is_unary_pre_operator(ctx.current())) {
    return expr_unary_state;
  }

  switch (kind) {
    case LexicalKind::KeywordTrue:
    case LexicalKind::KeywordFalse:
      return expr_bool_state;
    case LexicalKind::Number:
      return expr_number_state;
    case LexicalKind::String:
      return expr_string_state;
    case LexicalKind::Identifier:
      return expr_ident_state;
    default:
      throw_not_implemented();
  }
}

auto expr_start_handler_(ParserContext& ctx) -> ParserState { return expr_unknown_state; }
