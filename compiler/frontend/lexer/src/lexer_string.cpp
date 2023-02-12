// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer_string.hpp"

auto string_unexpected_end_error_handler_(LexerContext& ctx) -> LexerState;
auto string_end_handler_(LexerContext& ctx) -> LexerState;
auto string_escaped_handler_(LexerContext& ctx) -> LexerState;
auto string_hander_(LexerContext& ctx) -> LexerState;
auto string_start_handler_(LexerContext& ctx) -> LexerState;

constexpr LexerState string_unexpected_end_error_state =
    LexerState{string_unexpected_end_error_handler_};

constexpr LexerState string_end_state     = LexerState{string_end_handler_};
constexpr LexerState string_escaped_state = LexerState{string_escaped_handler_};
constexpr LexerState string_state         = LexerState{string_hander_};
constexpr LexerState string_start_state   = LexerState{string_start_handler_};

constexpr auto is_doublequote(char c) -> bool { return c == '"'; }
constexpr auto is_escape(char c) -> bool { return c == '\\'; }

auto string_unexpected_end_error_handler_(LexerContext& ctx) -> LexerState {
  throw_not_implemented();
}

auto string_end_handler_(LexerContext& ctx) -> LexerState {
  assert(is_doublequote(ctx.current()));
  create_value_token(ctx, LexicalKind::String);
  return ctx.move_next_state(unknown_state, exit_state);
}

auto string_escaped_handler_(LexerContext& ctx) -> LexerState {
  ctx.buffer_current();
  return ctx.move_next_state(string_state, string_unexpected_end_error_state);
}

// todo : implement

auto string_hander_(LexerContext& ctx) -> LexerState {
  constexpr auto conditions = std::array<LexerMatchCondition, 2>{
      LexerMatchCondition{is_doublequote, string_end_state    },
      LexerMatchCondition{is_escape,      string_escaped_state}
  };

  ctx.buffer_current();
  return ctx.move_next_state(string_state, string_unexpected_end_error_state, conditions);
}

auto string_start_handler_(LexerContext& ctx) -> LexerState {
  assert(is_doublequote(ctx.current()));
  ctx.mark_start_of_token();
  return ctx.move_next_state(
      is_doublequote, string_end_state, string_state, string_unexpected_end_error_state);
}