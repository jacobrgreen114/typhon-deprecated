// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include <iostream>
#include <memory>
#include <vector>

#ifdef TRACE
#include "../xml/serialization.hpp"
#endif

using String = std::shared_ptr<std::string>;

class FilePosition final {
  using int_type = size_t;

  int_type line_;
  int_type col_;

 public:
  FilePosition(int_type line, int_type col) : line_{line}, col_{col} {}
  FilePosition() : FilePosition{1, 0} {}

  NODISCARD constexpr auto line() const -> auto& { return line_; }
  NODISCARD constexpr auto col() const -> auto& { return col_; }

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

  // var
  KeywordVar,
  // func
  KeywordFunc,
  // struct
  KeywordStruct,
  // object
  KeywordObject,

  // concept
  KeywordConcept,
  // interface
  KeywordInterface,

  // return
  KeywordReturn,

  // if
  KeywordIf,
  // elif
  KeywordElif,
  // else
  KeywordElse,

  // switch
  KeywordSwitch,
  // match
  KeywordMatch,

  // loop
  KeywordLoop,
  // while
  KeywordWhile,
  // for
  KeywordFor,
  // foreach
  KeywordForeach,

  // .
  SymbolPeriod,
  // ;
  SymbolSemicolon,
  // :
  SymbolColon,
  // ,
  SymbolComma,

  // (
  SymbolParenOpen,
  // )
  SymbolParenClose,

  // [
  SymbolBraceOpen,
  // ]
  SymbolBraceClose,

  // <
  SymbolAngleOpen,
  // >
  SymbolAngleClose,

  // {
  SymbolBracketOpen,
  // }
  SymbolBracketClose,

  // ->
  SymbolArrow,

  // =
  SymbolEquals,

  // ==
  SymbolBoolEquals,
  // !=
  SymbolBoolNotEquals,
  // !
  SymbolBoolNot,
  // ||
  SymbolBoolOr,
  // &&
  SymbolBoolAnd,

  // ~
  SymbolBitNot,
  // |
  SymbolBitOr,
  // &
  SymbolBitAnd,
  // ^
  SymbolBitXor,

  // |=
  SymbolBitOrEquals,
  // &=
  SymbolBitAndEquals,
  // ^=
  SymbolBitXorEquals,

  // +
  SymbolPlus,
  // -
  SymbolMinus,
  // *
  SymbolStar,
  // /
  SymbolSlash,

  // ++
  SymbolInc,
  // --
  SymbolDec,

  // +=
  SymbolPlusEquals,
  // -=
  SymbolMinusEquals,
  // *=
  SymbolStarEquals,
  // /=
  SymbolSlashEquals,

  // <<
  SymbolShiftLeft,
  // >>
  SymbolShiftRight,

  // <=
  SymbolLessThanEqual,
  // >=
  SymbolGreaterThanEqual,
};

auto operator<<(std::ostream& stream, LexicalKind kind) -> std::ostream&;

class LexicalToken final
#ifdef TRACE
    : public xml::Serializable
#endif
{
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

  NODISCARD constexpr auto pos() const -> auto& { return pos_; }
  NODISCARD constexpr auto kind() const -> auto& { return kind_; }
  NODISCARD constexpr auto value() const -> auto& { return value_; }
  NODISCARD auto has_value() const -> bool { return static_cast<bool>(value_); }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

using TokenCollection = std::vector<LexicalToken>;

auto operator<<(std::ostream& stream, const LexicalToken& token)
    -> std::ostream&;
