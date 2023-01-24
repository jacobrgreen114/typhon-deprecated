// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include <iostream>
#include <memory>
#include <vector>

using String = std::shared_ptr<std::string>;

class FilePosition final {
  using int_type = size_t;

  int_type line_;
  int_type col_;

 public:
  FilePosition(int_type line, int_type col) : line_{line}, col_{col} {}
  FilePosition() : FilePosition{1, 0} {}

  pure constexpr auto line() const -> auto& { return line_; }
  pure constexpr auto col() const -> auto& { return col_; }

  constexpr auto next() -> void { ++col_; }
  constexpr auto nextline() -> void {
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

  KeywordReturn,

  KeywordIf,
  KeywordElif,
  KeywordElse,

  KeywordSwitch,
  KeywordMatch,

  KeywordLoop,
  KeywordWhile,
  KeywordFor,
  KeywordForeach,

  SymbolSemicolon,
  SymbolColon,
  SymbolComma,

  SymbolParenOpen,
  SymbolParenClose,

  SymbolBraceOpen,
  SymbolBraceClose,

  SymbolAngleOpen,
  SymbolAngleClose,

  SymbolBracketOpen,
  SymbolBracketClose,

  SymbolArrow,

  SymbolEquals,

  SymbolBoolEquals,
  SymbolBoolNotEquals,
  SymbolBoolNot,
  SymbolBoolOr,
  SymbolBoolAnd,

  SymbolBitNot,
  SymbolBitOr,
  SymbolBitAnd,
  SymbolBitXor,

  SymbolPlus,
  SymbolPlusPlus,
  SymbolPlusEquals,

  SymbolMinus,
  SymbolMinusMinus,
  SymbolMinusEquals,

  SymbolStar,
  SymbolStarEquals,

  SymbolSlash,
  SymbolSlashEquals,
};

auto operator<<(std::ostream& stream, LexicalKind kind) -> std::ostream&;

class LexicalToken final {
 public:
  using ValueType = String;

 private:
  const FilePosition pos_;
  const LexicalKind kind_;
  const ValueType value_;

 public:
  LexicalToken(const FilePosition& pos, LexicalKind kind, ValueType value)
      : pos_{pos}, kind_{kind}, value_{std::move(value)} {}

  LexicalToken(const FilePosition& pos, LexicalKind kind)
      : LexicalToken{pos, kind, nullptr} {}

  pure constexpr auto pos() const -> auto& { return pos_; }
  pure constexpr auto kind() const -> auto& { return kind_; }
  pure constexpr auto value() const -> auto& {
    return value_ ? value_ : throw std::exception();
  }
  pure auto has_value() const -> bool { return static_cast<bool>(value_); }
};

using TokenCollection = std::vector<LexicalToken>;

auto operator<<(std::ostream& stream, const LexicalToken& token)
    -> std::ostream&;