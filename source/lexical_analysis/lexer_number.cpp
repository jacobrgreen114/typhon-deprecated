// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer_number.hpp"

auto number_end_exit_handler_(LexerContext& ctx) -> LexerState;
auto number_end_handler_(LexerContext& ctx) -> LexerState;
auto number_handler_(LexerContext& ctx) -> LexerState;
auto number_start_handler_(LexerContext& ctx) -> LexerState;

constexpr auto number_end_exit_state    = LexerState{number_end_exit_handler_};
constexpr auto number_end_state         = LexerState{number_end_handler_};
constexpr LexerState number_state       = LexerState{number_handler_};
constexpr LexerState number_start_state = LexerState{number_start_handler_};

constexpr auto create_number_token(LexerContext& ctx) -> void {
  create_value_token(ctx, LexicalKind::Number);
}

auto number_end_exit_handler_(LexerContext& ctx) -> LexerState {
  create_number_token(ctx);
  return exit_state;
}

auto number_end_handler_(LexerContext& ctx) -> LexerState {
  create_number_token(ctx);
  return unknown_state;
}

auto number_handler_(LexerContext& ctx) -> LexerState {
#if LEXER_LOOP_OPTIMIZATION
  do {
    if (!matches_number(ctx.current())) {
      return number_end_state;
    }
    ctx.buffer_current();
  } while (ctx.move_next());
  return number_end_exit_state;
#else
  ctx.buffer_current();
  return ctx.move_next_state(matches_number, number_state, number_end_state, number_end_exit_state);
#endif
}

auto number_start_handler_(LexerContext& ctx) -> LexerState {
  ctx.mark_start_of_token();
  return number_state;
}