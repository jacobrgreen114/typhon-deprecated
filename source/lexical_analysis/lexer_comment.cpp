
// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "lexer_comment.hpp"

constexpr auto is_newline(char c) -> bool { return c == '\n'; }
constexpr auto is_star(char c) -> bool { return c == '*'; }
constexpr auto is_slash(char c) -> bool { return c == '/'; }

constexpr LexerState comment_singleline_end_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      return ctx.move_next_state(unknown_state, exit_state);
    }};

constexpr LexerState comment_singleline_start_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      return ctx.move_next_state(is_newline, comment_singleline_end_state,
                                 comment_singleline_start_state, exit_state);
    }};

constexpr LexerState comment_multiline_unexpected_end_error_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr LexerState comment_multiline_end_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      return ctx.move_next_state(unknown_state, exit_state);
    }};

extern const LexerState comment_multiline_state;

constexpr LexerState comment_multiline_possible_end_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      const auto conditions = std::array<LexerMatchCondition, 2>{
          LexerMatchCondition{is_slash, comment_multiline_end_state},
          LexerMatchCondition{is_star, comment_multiline_possible_end_state}};

      return ctx.move_next_state(comment_multiline_state,
                                 comment_multiline_unexpected_end_error_state,
                                 conditions);
    }};

constexpr LexerState comment_multiline_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      return ctx.move_next_state(is_star, comment_multiline_possible_end_state,
                                 comment_multiline_state,
                                 comment_multiline_unexpected_end_error_state);
    }};

constexpr LexerState comment_multiline_start_state = LexerState{
    [](LexerContext &ctx) -> LexerState { return comment_multiline_state; }};