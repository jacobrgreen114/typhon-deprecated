// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "lexer_number.hpp"

constexpr auto create_number_token(LexerContext &ctx) -> void {
  create_value_token(ctx, LexicalKind::Number);
}

constexpr auto number_end_exit_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      create_number_token(ctx);
      return exit_state;
    }};

constexpr auto number_end_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      create_number_token(ctx);
      return unknown_state;
    }};

constexpr LexerState number_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.buffer_current();
      return ctx.move_next_state(matches_number, number_state, number_end_state,
                                 number_end_exit_state);
    }};

constexpr LexerState number_start_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.mark_start_of_token();
      return number_state;
    }};
