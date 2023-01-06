// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "lexical_analysis.hpp"

#include <exception>

#define throw_not_implemented() throw std::exception("not implemented!");

#pragma region Lexer Number States

constexpr auto number_end_exit_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr auto number_end_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr auto number_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr auto number_start_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

#pragma endregion

#pragma region Lexer Global States

constexpr auto error_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr auto exit_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr auto unknown_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr auto start_state = LexerState{[](LexerContext &ctx) -> LexerState {
  return ctx.move_next_state(unknown_state, exit_state);
}};

#pragma endregion

#pragma region Lexer Context

constexpr auto null_char = '\0';

LexerContext::LexerContext(const std::string &path)
    : stream_{path}, current_{null_char} {}

auto LexerContext::move_next() -> bool {
  constexpr auto eof = std::ifstream::traits_type::eof();

  auto c = stream_.get();
  if (c == eof) {
    current_ = null_char;
    return false;
  }

  current_ = static_cast<char>(c);
  return true;
}

auto LexerContext::buffer_current() -> void { buffer_.push_back(current_); }

#pragma endregion

#pragma region Lexer

Lexer::Lexer() : StateMachine<LexerContext>{start_state} {}

#pragma endregion