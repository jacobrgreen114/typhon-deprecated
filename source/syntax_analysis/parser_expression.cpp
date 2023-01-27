// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "parser_expression.hpp"

// todo : implement postfix operators
// todo : convert period binary operator to access expression

/*
 * Error
 */

auto expr_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

constexpr ParserState expression_error_state = ParserState{expr_error_handler_};

auto expr_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

constexpr ParserState expr_unexpected_end_error_state =
    ParserState{expr_unexpected_end_error_handler_};

/*
 * Exit
 */

auto expr_exit_end_handler_(ParserContext& ctx) -> ParserState {
  return ctx.pop_end_state();
}

constexpr ParserState expr_exit_end_state = ParserState{expr_exit_end_handler_};

auto expr_exit_handler_(ParserContext& ctx) -> ParserState {
  return ctx.pop_ret_state();
}
constexpr ParserState expr_exit_state = ParserState{expr_exit_handler_};

auto expr_end_handler_(ParserContext& ctx) -> ParserState {
  // auto return_expr = ctx.expression_stack.top();
  // ctx.syntax_stack.push(return_expr);
  // ctx.expression_stack.pop();
  return expr_exit_state;
}
constexpr ParserState expr_end_state = ParserState{expr_end_handler_};

extern const ParserState expr_binary_state;
extern const ParserState expr_unknown_state;

auto expr_possible_end_handler_(ParserContext& ctx) -> ParserState {
  return ctx.move_next_state(is_binary_operator, expr_binary_state,
                             expr_exit_state, expr_exit_end_state);
}

constexpr ParserState expr_possible_end_state =
    ParserState{expr_possible_end_handler_};

/*
 * Number
 */

auto expr_number_handler_(ParserContext& ctx) -> ParserState {
  const auto& value = ctx.current().value();
  ctx.syntax_stack.push(std::make_shared<NumberExpression>(value));
  return expr_possible_end_state;
}
constexpr ParserState expr_number_state = ParserState{expr_number_handler_};

/*
 * Identifier
 */

// auto expr_ident_possible_end_handler_(ParserContext& ctx) -> ParserState {
//   throw_not_implemented();
// }
//
// constexpr ParserState expr_ident_possible_end_state =
//     ParserState{expr_ident_possible_end_handler_};

auto expr_ident_handler_(ParserContext& ctx) -> ParserState {
  const auto& value = ctx.current().value();
  ctx.syntax_stack.push(std::make_shared<IdentifierExpression>(value));
  return expr_possible_end_state;
}

constexpr ParserState expr_ident_state = ParserState{expr_ident_handler_};

/*
 * Binary
 */

auto do_expr_binary_end(ParserContext& ctx) -> void {
  ctx.precedence_stack.pop();

  auto rhs = ctx.pop_expr_node();
  auto op  = ctx.pop_expr_binary_node();
  auto lhs = ctx.pop_expr_node();

  op->set_lhs(lhs);
  op->set_rhs(rhs);

  ctx.syntax_stack.emplace(op);
}

auto expr_binary_end_exit_handler_(ParserContext& ctx) -> ParserState {
  do_expr_binary_end(ctx);
  return ctx.pop_end_state();
}

constexpr ParserState expr_binary_end_exit_state =
    ParserState{expr_binary_end_exit_handler_};

auto expr_binary_end_handler_(ParserContext& ctx) -> ParserState {
  do_expr_binary_end(ctx);
  if (is_binary_operator(ctx.current())) {
    return expr_binary_state;
  }

  return ctx.pop_ret_state();
}

constexpr ParserState expr_binary_end_state =
    ParserState{expr_binary_end_handler_};

auto expr_binary_handler_(ParserContext& ctx) -> ParserState {
  const auto& current = ctx.current();
  assert(is_binary_operator(current));

  const auto op         = get_binary_op(current.kind());
  const auto precedence = get_precedence(op);

  if (!ctx.precedence_stack.empty() &&
      ctx.precedence_stack.top() > precedence) {
    return ctx.pop_ret_state();
  }

  ctx.precedence_stack.emplace(precedence);
  ctx.syntax_stack.emplace(std::make_shared<BinaryExpression>(op));

  ctx.push_states(expr_binary_end_state, expr_binary_end_exit_state);
  return ctx.move_next_state(expr_unknown_state,
                             expr_unexpected_end_error_state);
}
constexpr ParserState expr_binary_state = ParserState{expr_binary_handler_};

/*
 * Unary
 */

void do_expr_unary_end(ParserContext& ctx) {
  ctx.precedence_stack.pop();
  auto expr   = ctx.pop_expr_node();
  auto* unary = ctx.get_expr_unary_node();
  unary->set_expr(expr);
}

ParserState expr_unary_end_exit_handler_(ParserContext& ctx) {
  do_expr_unary_end(ctx);
  return ctx.pop_end_state();
}

constexpr auto expr_unary_end_exit_state =
    ParserState{expr_unary_end_exit_handler_};

ParserState expr_unary_end_handler_(ParserContext& ctx) {
  do_expr_unary_end(ctx);
  // todo : redo unary and binary state entrances and exits
  if (is_binary_operator(ctx.current())) {
    return expr_binary_state;
  }
  return ctx.pop_ret_state();
}

constexpr auto expr_unary_end_state = ParserState{expr_unary_end_handler_};

ParserState expr_unary_handler_(ParserContext& ctx) {
  const auto& current = ctx.current();
  const auto op       = get_unary_pre_op(current.kind());

  const auto prec = get_precedence(op);
  ctx.precedence_stack.push(prec);

  ctx.syntax_stack.push(std::make_shared<UnaryExpression>(op));
  ctx.push_states(expr_unary_end_state, expr_unary_end_exit_state);
  return ctx.move_next_state(expr_unknown_state,
                             expr_unexpected_end_error_state);
}

constexpr auto expr_unary_state = ParserState{expr_unary_handler_};

/*
 * Start
 */

auto expr_unknown_handler_(ParserContext& ctx) -> ParserState {
  auto kind = ctx.current().kind();
  if (is_unary_pre_operator(ctx.current())) {
    return expr_unary_state;
  }

  switch (kind) {
    case LexicalKind::Number:
      return expr_number_state;
    case LexicalKind::Identifier:
      return expr_ident_state;
    default:
      throw_not_implemented();
  }
}
constexpr ParserState expr_unknown_state = ParserState{expr_unknown_handler_};

auto expr_start_handler_(ParserContext& ctx) -> ParserState {
  return expr_unknown_state;
}

constexpr ParserState expr_start_state = ParserState{expr_start_handler_};