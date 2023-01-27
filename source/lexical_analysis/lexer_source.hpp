// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "../state_machine.hpp"
#include "token.hpp"

#pragma region Lexer

class LexerContext;
using LexerState = State<LexerContext>;

class LexerContext final : public EnumeratingContext<LexerContext, char> {
  fs::path path_;
  std::ifstream stream_;
  char current_;
  FilePosition current_pos_;
  FilePosition token_pos_;
  std::string buffer_;

 public:
  TokenCollection tokens;

  explicit LexerContext(const fs::path& path);

  auto current() -> const char& override { return current_; }
  auto move_next() -> bool override;

  NODISCARD constexpr auto token_position() const -> auto& {
    return token_pos_;
  }

  auto buffer_current() -> void;
  auto mark_start_of_token() -> void;

  constexpr auto pop_buffer() { return std::move(buffer_); }
};

using LexerMatchCondition = LexerContext::MatchCondition;
using LexerRefMatchCondition = LexerContext::RefMatchCondition;

class Lexer : public StateMachine<LexerContext> {
 public:
  Lexer();
};

#pragma endregion

constexpr auto create_empty_token(LexerContext& ctx, LexicalKind kind) {
  ctx.tokens.emplace_back(ctx.token_position(), kind);
}

constexpr auto create_value_token(LexerContext& ctx, LexicalKind kind) {
  ctx.tokens.emplace_back(ctx.token_position(), kind,
                          std::make_shared<std::string>(ctx.pop_buffer()));
}

constexpr auto should_match_whitespace(const char c) -> bool {
  return ('\0' < c and c <= ' ');
}

constexpr auto matches_whitespace(const char c) -> bool {
  return should_match_whitespace(c);
}

extern const LexerState error_state;
extern const LexerState exit_state;
extern const LexerState unknown_state;