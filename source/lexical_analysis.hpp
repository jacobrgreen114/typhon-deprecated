// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#ifdef __cplusplus

#pragma once

#include <fstream>
#include <string>

#include "state_machine.hpp"

#pragma region Misc

class FilePosition final {
  int line_;
  int col_;
};

#pragma endregion

#pragma region Token

enum class LexicalKind {
  Unknown,

  Number,
  Identifier
};

class LexicalToken final {
  const FilePosition pos_;
  const LexicalKind kind_;
  const std::unique_ptr<std::string> value_;

 public:
  LexicalToken(const FilePosition& pos, LexicalKind kind,
               std::unique_ptr<std::string>&& value)
      : pos_{pos},
        kind_{kind},
        value_{std::forward<std::unique_ptr<std::string>>(value)} {}

  LexicalToken(const FilePosition& pos, LexicalKind kind)
      : LexicalToken{pos, kind, nullptr} {}
};

#pragma endregion

#pragma region Lexer

class LexerContext;
using LexerState = State<LexerContext>;

class LexerContext final : public EnumeratingContext<LexerContext, char> {
  std::ifstream stream_;
  char current_;
  FilePosition current_pos_;
  FilePosition token_pos_;
  std::string buffer_;

 public:
  explicit LexerContext(const std::string& path);

  auto current() -> const char& override { return current_; }
  auto move_next() -> bool override;

  auto buffer_current() -> void;
};

class Lexer : public StateMachine<LexerContext> {
 public:
  Lexer();
};

#pragma endregion

#endif