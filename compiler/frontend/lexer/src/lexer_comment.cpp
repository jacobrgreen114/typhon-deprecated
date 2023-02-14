
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer_comment.hpp"

constexpr auto is_newline(char c) -> bool { return c == '\n'; }
constexpr auto is_star(char c) -> bool { return c == '*'; }
constexpr auto is_slash(char c) -> bool { return c == '/'; }

/*
 * Singleline Comment
 */

auto comment_singleline_end_handler_(LexerContext& ctx) -> LexerState;
auto comment_singleline_handler_(LexerContext& ctx) -> LexerState;

constexpr LexerState comment_singleline_end_state   = LexerState{comment_singleline_end_handler_};
constexpr LexerState comment_singleline_start_state = LexerState{comment_singleline_handler_};

auto comment_singleline_end_handler_(LexerContext& ctx) -> LexerState {
  return ctx.move_next_state(unknown_state, exit_state);
}

auto comment_singleline_handler_(LexerContext& ctx) -> LexerState {
#if LEXER_LOOP_OPTIMIZATION
  while (ctx.move_next() && ctx.current() != '\n') {
  }
  return ctx.current() != eof_char ? unknown_state : exit_state;
#else
  return ctx.move_next_state(
      is_newline, comment_singleline_end_state, comment_singleline_start_state, exit_state);
#endif
}

/*
 * Multiline Comment
 */

auto comment_multiline_unexpected_end_error_handler_(LexerContext& ctx) -> LexerState;

auto comment_multiline_end_handler_(LexerContext& ctx) -> LexerState;
auto comment_multiline_possible_end_handler_(LexerContext& ctx) -> LexerState;

auto comment_multiline_handler_(LexerContext& ctx) -> LexerState;

constexpr LexerState comment_multiline_unexpected_end_error_state =
    LexerState{comment_multiline_unexpected_end_error_handler_};

constexpr LexerState comment_multiline_end_state = LexerState{comment_multiline_end_handler_};
constexpr LexerState comment_multiline_possible_end_state =
    LexerState{comment_multiline_possible_end_handler_};

constexpr LexerState comment_multiline_state       = LexerState{comment_multiline_handler_};

constexpr LexerState comment_multiline_start_state = LexerState{comment_multiline_handler_};

auto comment_multiline_unexpected_end_error_handler_(LexerContext& ctx) -> LexerState {
  throw_not_implemented();
}

auto comment_multiline_end_handler_(LexerContext& ctx) -> LexerState {
  return ctx.move_next_state(unknown_state, exit_state);
}

auto comment_multiline_possible_end_handler_(LexerContext& ctx) -> LexerState {
#if LEXER_LOOP_OPTIMIZATION
  if (ctx.move_next() && ctx.current() == '/') {
    return ctx.move_next_state(unknown_state, exit_state);
  }

  return ctx.current() != eof_char ? comment_multiline_state
                                   : comment_multiline_unexpected_end_error_state;
#else
  static constexpr auto conditions = std::array<LexerMatchCondition, 2>{
      LexerMatchCondition{is_slash, comment_multiline_end_state         },
      LexerMatchCondition{is_star,  comment_multiline_possible_end_state}
  };

  return ctx.move_next_state(
      comment_multiline_state, comment_multiline_unexpected_end_error_state, conditions);
#endif
}

auto comment_multiline_handler_(LexerContext& ctx) -> LexerState {
#if LEXER_LOOP_OPTIMIZATION
  while (ctx.move_next() && ctx.current() != '*') {
  }
  return ctx.current() == '*' ? comment_multiline_possible_end_state
                              : comment_multiline_unexpected_end_error_state;
#else
  return ctx.move_next_state(is_star,
                             comment_multiline_possible_end_state,
                             comment_multiline_state,
                             comment_multiline_unexpected_end_error_state);
#endif
}
