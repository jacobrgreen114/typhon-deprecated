// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer_sm.hpp"

#include "lexer_identifier.hpp"
#include "lexer_symbol.hpp"
#include "lexer_number.hpp"
#include "lexer_string.hpp"

/*
 * Exit States
 */

auto error_handler_(LexerContext& ctx) -> LexerState;

constexpr LexerState error_state = LexerState{error_handler_};
constexpr LexerState exit_state  = LexerState{nullptr};

auto error_handler_(LexerContext& ctx) -> LexerState { throw_not_implemented(); }

/*
 * Whitespace
 */

auto whitespace_handler_(LexerContext& ctx) -> LexerState;
//auto whitespace_start_handler_(LexerContext& ctx) -> LexerState;

constexpr LexerState whitespace_state       = LexerState{whitespace_handler_};
constexpr LexerState whitespace_start_state = LexerState{whitespace_handler_};

auto whitespace_handler_(LexerContext& ctx) -> LexerState {
#if LEXER_LOOP_OPTIMIZATION
  do {
    if (!matches_whitespace(ctx.current())) {
      return unknown_state;
    }
  } while (ctx.move_next());
  return exit_state;
#else
  return ctx.move_next_state(matches_whitespace, whitespace_state, unknown_state, exit_state);
#endif
}

/*
 * States
 */
auto unknown_handler_(LexerContext& ctx) -> LexerState;
auto start_handler_(LexerContext& ctx) -> LexerState;

constexpr LexerState unknown_state = LexerState{unknown_handler_};
constexpr auto start_state         = LexerState{start_handler_};

auto unknown_handler_(LexerContext& ctx) -> LexerState {
  const auto& current = ctx.current();

  if (should_match_whitespace(current)) {
    return whitespace_start_state;
  }
  if (should_match_identifier(current)) {
    return identifier_start_state;
  }
  if (should_match_number(current)) {
    return number_start_state;
  }
  if (should_match_string(current)) {
    return string_start_state;
  }
  if (should_match_symbol(current)) {
    return symbol_start_state;
  }

  return error_state;
}

auto start_handler_(LexerContext& ctx) -> LexerState {
  return ctx.move_next_state(unknown_state, exit_state);
}

#pragma endregion

#pragma region Lexer Context

constexpr auto null_char = '\0';

LexerContext::LexerContext(const fs::path& path)
    : path_{path},
      stream_{path},
      current_{null_char} {
  if (stream_.fail()) {
    throw std::exception("failed to open file.");
  }
}

auto LexerContext::move_next() -> bool {
  constexpr auto eof = std::ifstream::traits_type::eof();

  auto c             = stream_.get();
  if (c == eof) {
    current_ = null_char;
    return false;
  }

  current_ = static_cast<char>(c);
  if (current_ == '\n') {
    current_pos_.nextline();
  } else {
    current_pos_.next();
  }

  return true;
}

auto LexerContext::buffer_current() -> void { buffer_.push_back(current_); }

auto LexerContext::mark_start_of_token() -> void { token_pos_ = current_pos_; }

#pragma endregion

#pragma region Lexer

Lexer::Lexer()
    : StateMachine<LexerContext>{start_state} {}

#pragma endregion