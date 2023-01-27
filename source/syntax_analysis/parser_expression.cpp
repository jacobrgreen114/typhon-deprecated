// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "parser_expression.hpp"

// todo : implement proper precedence handling

auto expr_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

constexpr ParserState expression_error_state = ParserState{expr_error_handler_};

auto expr_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

constexpr ParserState expression_unexpected_end_error_state =
    ParserState{expr_unexpected_end_error_handler_};

auto expr_exit_end_handler_(ParserContext& ctx) -> ParserState {
  auto ret_states = ctx.pop_states();
  return ret_states.end;
}

constexpr ParserState expression_exit_end_state =
    ParserState{expr_exit_end_handler_};

auto expr_exit_handler_(ParserContext& ctx) -> ParserState {
  auto ret_states = ctx.pop_states();
  return ret_states.ret;
}
constexpr ParserState expression_exit_state = ParserState{expr_exit_handler_};

auto expr_end_handler_(ParserContext& ctx) -> ParserState {
  // auto return_expr = ctx.expression_stack.top();
  // ctx.syntax_stack.push(return_expr);
  // ctx.expression_stack.pop();
  return expression_exit_state;
}
constexpr ParserState expr_end_state = ParserState{expr_end_handler_};

extern const ParserState expr_binary_state;
extern const ParserState expr_unknown_state;

auto expr_possible_end_handler_(ParserContext& ctx) -> ParserState {
  return ctx.move_next_state(is_binary_operator, expr_binary_state,
                             expression_exit_state, expression_exit_end_state);
}

constexpr ParserState expr_possible_end_state =
    ParserState{expr_possible_end_handler_};

auto do_expr_binary_end(ParserContext& ctx) -> void {
  auto rhs = ctx.pop_expr_node();
  auto op = ctx.pop_expr_binary_node();
  auto lhs = ctx.pop_expr_node();

  op->set_lhs(lhs);
  op->set_rhs(rhs);

  ctx.syntax_stack.emplace(op);
}

auto expr_binary_end_exit_handler_(ParserContext& ctx) -> ParserState {
  do_expr_binary_end(ctx);
  auto ret_states = ctx.pop_states();
  return ret_states.end;
}

constexpr ParserState expr_binary_end_exit_state =
    ParserState{expr_binary_end_exit_handler_};

auto expr_binary_end_handler_(ParserContext& ctx) -> ParserState {
  do_expr_binary_end(ctx);
  auto ret_states = ctx.pop_states();
  return ret_states.ret;
}

constexpr ParserState expr_binary_end_state =
    ParserState{expr_binary_end_handler_};

auto expr_binary_handler_(ParserContext& ctx) -> ParserState {
  const auto& current = ctx.current();
  assert(is_binary_operator(current));

  const auto op = get_binary_op(current.kind());

  //const auto precedence = get_precedence(op);
  //if (!ctx.precedence_stack.empty()) {
  //  if (ctx.precedence_stack.top() > precedence) {
  //    auto states = ctx.pop_states();
  //    return states.ret;
  //  }
  //}
  //ctx.precedence_stack.emplace(precedence);

  ctx.syntax_stack.emplace(std::make_shared<BinaryExpression>(op));

  ctx.push_states(expr_binary_end_state, expr_binary_end_exit_state);
  return ctx.move_next_state(expr_unknown_state,
                             expression_unexpected_end_error_state);
}
constexpr ParserState expr_binary_state = ParserState{expr_binary_handler_};

auto expr_number_handler_(ParserContext& ctx) -> ParserState {
  const auto& value = ctx.current().value();
  ctx.syntax_stack.push(std::make_shared<ConstantNumberExpressionNode>(value));
  return expr_possible_end_state;
}
constexpr ParserState expr_number_state = ParserState{expr_number_handler_};

constexpr auto expr_unknown_handler_(ParserContext& ctx) -> ParserState {
  auto kind = ctx.current().kind();
  switch (kind) {
    case LexicalKind::Number:
      return expr_number_state;
    default:
      throw_not_implemented();
  }
}
constexpr ParserState expr_unknown_state = ParserState{expr_unknown_handler_};

constexpr ParserState expr_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      //ctx.push_states(expression_exit_state, expression_error_state);
      return expr_unknown_state;
    }};