// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "expression_parsing.hpp"

constexpr ParserState expression_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

constexpr ParserState expression_exit_end_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto ret_states = ctx.pop_states();
      return ret_states.end;
    }};

constexpr ParserState expression_exit_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto ret_states = ctx.pop_states();
      return ret_states.ret;
    }};

constexpr ParserState expression_end_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      // auto return_expr = ctx.expression_stack.top();
      // ctx.syntax_stack.push(return_expr);
      // ctx.expression_stack.pop();
      return expression_exit_state;
    }};

extern const ParserState expression_binary_state;

constexpr ParserState expression_possible_end_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      return ctx.move_next_state(is_binary_operator, expression_binary_state,
                                 expression_exit_state,
                                 expression_exit_end_state);
    }};

constexpr ParserState expression_binary_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      const auto& current = ctx.current();
      assert(is_binary_operator(current));

      const auto op = get_binary_op(current.kind());

      throw_not_implemented();
    }};

constexpr ParserState expression_number_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      const auto& value = ctx.current().value();
      ctx.expression_stack.push(
          std::make_shared<ConstantNumberExpressionNode>(value));
      return expression_possible_end_state;
    }};

constexpr ParserState expression_unknown_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto kind = ctx.current().kind();
      switch (kind) {
        case LexicalKind::Number:
          return expression_number_state;
        default:
          throw_not_implemented();
      }
    }};

constexpr ParserState expression_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      ctx.push_states(expression_exit_state, expression_error_state);
      return expression_unknown_state;
    }};