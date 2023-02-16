// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "source.hpp"

#include "xml/rapid_xml.hpp"

#ifdef TRACE
#include "xml/serialization.hpp"
#endif

// using String = std::unique_ptr<const std::string>;

/*
 * Lexical Kind
 */

using lexical_kind_t = uint32_t;

enum class LexicalType {
  Misc       = 0x00,
  Keyword    = 0x01,
  Symbol     = 0x02,
  Constant   = 0x03,
  Identifier = 0x04,
};

constexpr auto lexical_kind_type_offset  = 24;
constexpr auto lexical_kind_type_bitmask = 0xff;
constexpr auto lexical_kind_type_mask    = lexical_kind_type_bitmask << lexical_kind_type_offset;

constexpr auto lexical_kind_unary_flag_offset  = 16;
constexpr auto lexical_kind_unary_flag_bitmask = 0b1;
constexpr auto lexical_kind_unary_flag_mask    = lexical_kind_unary_flag_bitmask
                                              << lexical_kind_unary_flag_offset;

constexpr auto lexical_kind_binary_flag_offset  = 17;
constexpr auto lexical_kind_binary_flag_bitmask = 0b1;
constexpr auto lexical_kind_binary_flag_mask    = lexical_kind_binary_flag_bitmask
                                               << lexical_kind_binary_flag_offset;

constexpr auto lexical_kind_ternary_flag_offset  = 18;
constexpr auto lexical_kind_ternary_flag_bitmask = 0b1;
constexpr auto lexical_kind_ternary_flag_mask    = lexical_kind_ternary_flag_bitmask
                                                << lexical_kind_ternary_flag_offset;

constexpr auto lexical_kind_prefix_flag_offset  = 19;
constexpr auto lexical_kind_prefix_flag_bitmask = 0b1;
constexpr auto lexical_kind_prefix_flag_mask    = lexical_kind_prefix_flag_bitmask
                                               << lexical_kind_prefix_flag_offset;

constexpr auto lexical_kind_postfix_flag_offset  = 20;
constexpr auto lexical_kind_postfix_flag_bitmask = 0b1;
constexpr auto lexical_kind_postfix_flag_mask    = lexical_kind_postfix_flag_bitmask
                                                << lexical_kind_postfix_flag_offset;

constexpr auto make_lexical_kind(LexicalType type, lexical_kind_t value) -> lexical_kind_t {
  return (static_cast<std::underlying_type_t<LexicalType>>(type) << lexical_kind_type_offset) |
         value;
}

constexpr auto make_lexical_kind_keyword(lexical_kind_t value) -> lexical_kind_t {
  return make_lexical_kind(LexicalType::Keyword, value);
}
constexpr auto make_lexical_kind_constant(lexical_kind_t value) -> lexical_kind_t {
  return make_lexical_kind(LexicalType::Constant, value);
}

constexpr auto make_lexical_kind_operator(
    bool unary, bool binary, bool ternary, bool prefix, bool postfix, lexical_kind_t value)
    -> lexical_kind_t {
  return make_lexical_kind(
      LexicalType::Symbol,
      (static_cast<lexical_kind_t>(unary) << lexical_kind_unary_flag_offset) |
          (static_cast<lexical_kind_t>(binary) << lexical_kind_binary_flag_offset) |
          (static_cast<lexical_kind_t>(ternary) << lexical_kind_ternary_flag_offset) |
          (static_cast<lexical_kind_t>(prefix) << lexical_kind_prefix_flag_offset) |
          (static_cast<lexical_kind_t>(postfix) << lexical_kind_postfix_flag_offset) | value);
}

// todo : pack flags into enum value
enum class LexicalKind : lexical_kind_t {
  Unknown,

  Identifier       = make_lexical_kind(LexicalType::Identifier, 0x01),
  Number           = make_lexical_kind_constant(0x0001),
  String           = make_lexical_kind_constant(0x0002),

  // true
  KeywordTrue      = make_lexical_kind_keyword(0x0001),
  // true
  KeywordFalse     = make_lexical_kind_keyword(0x0002),

  // var
  KeywordVar       = make_lexical_kind_keyword(0x0011),
  // func
  KeywordFunc      = make_lexical_kind_keyword(0x0012),
  // struct
  KeywordStruct    = make_lexical_kind_keyword(0x0013),
  // object
  KeywordObject    = make_lexical_kind_keyword(0x0014),
  // concept
  KeywordConcept   = make_lexical_kind_keyword(0x0015),
  // interface
  KeywordInterface = make_lexical_kind_keyword(0x0016),

  // return
  KeywordReturn    = make_lexical_kind_keyword(0x0021),
  // if
  KeywordIf        = make_lexical_kind_keyword(0x0022),
  // elif
  KeywordElif      = make_lexical_kind_keyword(0x0023),
  // else
  KeywordElse      = make_lexical_kind_keyword(0x0024),

  // switch
  KeywordSwitch    = make_lexical_kind_keyword(0x0031),
  // match
  KeywordMatch     = make_lexical_kind_keyword(0x0032),

  // loop
  KeywordLoop      = make_lexical_kind_keyword(0x0041),
  // while
  KeywordWhile     = make_lexical_kind_keyword(0x0042),
  // for
  KeywordFor       = make_lexical_kind_keyword(0x0043),
  // foreach
  KeywordForeach   = make_lexical_kind_keyword(0x0044),

  // namespace
  KeywordNamespace = make_lexical_kind_keyword(0x0051),
  // import
  KeywordImport    = make_lexical_kind_keyword(0x0052),

  // private
  KeywordPrivate   = make_lexical_kind_keyword(0x0061),
  // module
  KeywordModule    = make_lexical_kind_keyword(0x0062),
  // internal
  KeywordInternal  = make_lexical_kind_keyword(0x0063),
  // protected
  KeywordProtected = make_lexical_kind_keyword(0x0064),
  // public
  KeywordPublic    = make_lexical_kind_keyword(0x0065),

  // static
  KeywordStatic    = make_lexical_kind_keyword(0x0071),
  // mut
  KeywordMutable   = make_lexical_kind_keyword(0x0072),

  // .
  SymbolPeriod,
  // ;
  SymbolSemicolon,
  // :
  SymbolColon,
  // ::
  SymbolDoubleColon,
  // ,
  SymbolComma,

  // (
  SymbolParenOpen,
  // )
  SymbolParenClose,

  // [
  SymbolSquareOpen,
  // ]
  SymbolsquareClose,

  // <
  SymbolAngleOpen,
  // >
  SymbolAngleClose,

  // {
  SymbolCurlyOpen,
  // }
  SymbolCurlyClose,

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

  // Internal Symbols

  KeywordCType,
  KeywordCInclude,
};

auto operator<<(std::ostream& stream, LexicalKind kind) -> std::ostream&;

constexpr auto get_lexical_type(LexicalKind kind) -> LexicalType {
  return static_cast<LexicalType>(
      (static_cast<std::underlying_type_t<LexicalKind>>(kind) & lexical_kind_type_mask) >>
      lexical_kind_type_offset);
}

/**
 * File Position
 */
class FilePosition final {
  using int_type = size_t;

  int_type line_;
  int_type col_;

 public:
  FilePosition(int_type line = 1, int_type col = 0);

  NODISCARD constexpr auto line() const -> auto& { return line_; }
  NODISCARD constexpr auto col() const -> auto& { return col_; }

  NODISCARD auto to_string() const -> std::string;

  constexpr auto next() -> void { ++col_; }
  constexpr auto nextline() -> void {
    ++line_;
    col_ = 0;
  }

  NODISCARD auto xml_create_attr(xml::document& doc) const -> xml::attr&;
};

auto operator<<(std::ostream& stream, const FilePosition& position) -> std::ostream&;

/**
 * LexicalToken
 */
class LexicalToken final {
 public:
  using ValueType = std::string;

 private:
  const FilePosition pos_;
  const LexicalKind kind_;
  const ValueType value_;

 public:
  LexicalToken(const FilePosition& pos, LexicalKind kind, ValueType value = {});

  NODISCARD constexpr auto& pos() const { return pos_; }
  NODISCARD constexpr auto& kind() const { return kind_; }
  NODISCARD constexpr auto& value() const { return value_; }

  NODISCARD constexpr auto has_value() const { return !value_.empty(); }

  NODISCARD auto xml_create_node(xml::document& doc) const -> xml::node&;
};

auto operator<<(std::ostream& stream, const LexicalToken& token) -> std::ostream&;

class TokenCollection {
  std::shared_ptr<SourceContext> source_;
  std::vector<LexicalToken> tokens_;

 public:
  explicit TokenCollection(SourceContext::Pointer source, std::vector<LexicalToken> tokens)
      : source_{std::move(source)},
        tokens_{std::move(tokens)} {}

  NODISCARD auto& source() const { return source_; }
  NODISCARD auto& tokens() const { return tokens_; }

  NODISCARD auto xml_create_node(xml::document& doc) const -> xml::node&;
};

auto operator<<(std::ostream& stream, const TokenCollection& token_collection) -> std::ostream&;
