// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#ifdef __cplusplus

#pragma once

#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "state_machine.hpp"

#define pure [[nodiscard]]

#pragma region     FilePosition

class FilePosition final {
  using int_type = size_t;

  int_type line_;
  int_type col_;

 public:
  FilePosition(int_type line, int_type col) : line_{line}, col_{col} {}
  FilePosition() : FilePosition{1, 0} {}

  pure constexpr auto line() const -> auto& { return line_; }
  pure constexpr auto col() const -> auto& { return col_; }

  constexpr auto      next() -> void { ++col_; }
  constexpr auto      nextline() -> void {
    ++line_;
    col_ = 0;
  }
};

auto operator<<(std::ostream& stream, const FilePosition& position)
    -> std::ostream&;

#pragma endregion

#pragma region Token

enum class LexicalKind {
  Unknown,

  Identifier,
  Number,

  KeywordVar,
  KeywordFunc,
  KeywordObject,

  SymbolSemicolon,
  SymbolColon,

  SymbolAssign,
  SymbolBooleanEquals,

};

auto operator<<(std::ostream& stream, LexicalKind kind) -> std::ostream&;

class LexicalToken final {
 public:
  using ValueType = std::shared_ptr<std::string>;

 private:
  const FilePosition pos_;
  const LexicalKind  kind_;
  const ValueType    value_;

 public:
  LexicalToken(const FilePosition& pos, LexicalKind kind, ValueType value)
      : pos_{pos}, kind_{kind}, value_{std::move(value)} {}

  LexicalToken(const FilePosition& pos, LexicalKind kind)
      : LexicalToken{pos, kind, nullptr} {}

  pure constexpr auto pos() const -> auto& { return pos_; }
  pure constexpr auto kind() const -> auto& { return kind_; }
  pure constexpr auto value() const -> auto& {
    return value_ ? *value_ : throw std::exception();
  }
  pure auto has_value() const -> bool { return static_cast<bool>(value_); }
};

auto operator<<(std::ostream& stream, const LexicalToken& token)
    -> std::ostream&;

#pragma endregion

#pragma region Lexer

class LexerContext;
using LexerState = State<LexerContext>;

class LexerContext final : public EnumeratingContext<LexerContext, char> {
  std::ifstream stream_;
  char          current_;
  FilePosition  current_pos_;
  FilePosition  token_pos_;
  std::string   buffer_;

 public:
  std::vector<LexicalToken> tokens;

  explicit LexerContext(const std::string& path);

  auto                current() -> const char& override { return current_; }
  auto                move_next() -> bool override;

  pure constexpr auto token_position() const -> auto& { return token_pos_; }

  auto                buffer_current() -> void;
  auto                mark_start_of_token() -> void;

  constexpr auto      pop_buffer() { return std::move(buffer_); }
};

class Lexer : public StateMachine<LexerContext> {
 public:
  Lexer();
};

#pragma endregion

#endif