// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "token.hpp"

const auto lexical_kind_names_ =
    std::unordered_map<LexicalKind, std::string_view>{
        {LexicalKind::Unknown, "Unknown"},

        {LexicalKind::Identifier, "Identifier"},
        {LexicalKind::Number, "Number"},

        {LexicalKind::KeywordVar, "KeywordVar"},
        {LexicalKind::KeywordFunc, "KeywordFunc"},
        {LexicalKind::KeywordStruct, "KeywordStruct"},
        {LexicalKind::KeywordObject, "KeywordObject"},

        {LexicalKind::KeywordConcept, "KeywordConcept"},
        {LexicalKind::KeywordInterface, "KeywordInterface"},

        {LexicalKind::KeywordReturn, "KeywordReturn"},

        {LexicalKind::KeywordIf, "KeywordIf"},
        {LexicalKind::KeywordElif, "KeywordElif"},
        {LexicalKind::KeywordElse, "KeywordElse"},

        {LexicalKind::KeywordSwitch, "KeywordSwitch"},
        {LexicalKind::KeywordMatch, "KeywordMatch"},

        {LexicalKind::KeywordLoop, "KeywordLoop"},
        {LexicalKind::KeywordWhile, "KeywordWhile"},
        {LexicalKind::KeywordFor, "KeywordFor"},
        {LexicalKind::KeywordForeach, "KeywordForeach"},

        {LexicalKind::SymbolSemicolon, "SymbolSemicolon"},
        {LexicalKind::SymbolColon, "SymbolColon"},
        {LexicalKind::SymbolComma, "SymbolComma"},

        {LexicalKind::SymbolParenOpen, "SymbolParenOpen"},
        {LexicalKind::SymbolParenClose, "SymbolParenClose"},

        {LexicalKind::SymbolBraceOpen, "SymbolBraceOpen"},
        {LexicalKind::SymbolBraceClose, "SymbolBraceClose"},

        {LexicalKind::SymbolAngleOpen, "SymbolAngleOpen"},
        {LexicalKind::SymbolAngleClose, "SymbolAngleClose"},

        {LexicalKind::SymbolBracketOpen, "SymbolBracketOpen"},
        {LexicalKind::SymbolBracketClose, "SymbolBracketClose"},

        {LexicalKind::SymbolArrow, "SymbolArrow"},

        {LexicalKind::SymbolEquals, "SymbolEquals"},

        {LexicalKind::SymbolBoolEquals, "SymbolBoolEquals"},
        {LexicalKind::SymbolBoolNotEquals, "SymbolBoolNotEquals"},
        {LexicalKind::SymbolBoolNot, "SymbolBoolNot"},
        {LexicalKind::SymbolBoolOr, "SymbolBoolOr"},
        {LexicalKind::SymbolBoolAnd, "SymbolBoolAnd"},

        {LexicalKind::SymbolBitNot, "SymbolBitNot"},
        {LexicalKind::SymbolBitOr, "SymbolBitOr"},
        {LexicalKind::SymbolBitAnd, "SymbolBitAnd"},
        {LexicalKind::SymbolBitXor, "SymbolBitXor"},

        {LexicalKind::SymbolPlus, "SymbolPlus"},
        {LexicalKind::SymbolMinus, "SymbolMinus"},
        {LexicalKind::SymbolStar, "SymbolStar"},
        {LexicalKind::SymbolSlash, "SymbolSlash"},

        {LexicalKind::SymbolShiftLeft, "SymbolShiftLeft"},
        {LexicalKind::SymbolShiftRight, "SymbolShiftRight"},

        {LexicalKind::SymbolLessThanEqual, "SymbolLessThanEqual"},
        {LexicalKind::SymbolGreaterThanEqual, "SymbolGreaterThanEqual"},

    };

auto operator<<(std::ostream &stream, LexicalKind kind) -> std::ostream & {
  const auto x = lexical_kind_names_.find(kind);
  if (x == lexical_kind_names_.end()) {
    throw std::exception("");
  }
  stream << x->second;
  return stream;
}

auto operator<<(std::ostream &stream, const FilePosition &position)
    -> std::ostream & {
  stream << '(' << position.line() << ", " << position.col() << ')';
  return stream;
}

auto operator<<(std::ostream &stream, const LexicalToken &token)
    -> std::ostream & {
  stream << R"({ "pos" : ")" << token.pos() << R"(", "kind" : ")"
         << token.kind() << R"(")";
  if (token.has_value()) {
    stream << R"(, "value" : ")" << *token.value() << R"(")";
  }

  stream << R"( })";
  return stream;
}
