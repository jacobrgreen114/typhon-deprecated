// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "lexical_analysis.hpp"

#include <exception>
#include <unordered_map>

#pragma region Lexical Kind

const auto lexical_kind_names_ =
    std::unordered_map<LexicalKind, std::string_view>{
        {LexicalKind::Unknown, "Unknown"},

        {LexicalKind::Identifier, "Identifier"},
        {LexicalKind::Number, "Number"},

        {LexicalKind::KeywordVar, "KeywordVar"},
        {LexicalKind::KeywordFunc, "KeywordFunc"},
        {LexicalKind::KeywordObject, "KeywordObject"},

        {LexicalKind::KeywordReturn, "KeywordReturn"},

        {LexicalKind::KeywordIf, "KeywordIf"},
        {LexicalKind::KeywordElif, "KeywordElif"},
        {LexicalKind::KeywordElse, "KeywordElse"},

        {LexicalKind::KeywordLoop, "KeywordLoop"},
        {LexicalKind::KeywordWhile, "KeywordWhile"},
        {LexicalKind::KeywordFor, "KeywordFor"},
        {LexicalKind::KeywordForeach, "KeywordForeach"},

        {LexicalKind::SymbolSemicolon, "SymbolSemicolon"},
        {LexicalKind::SymbolColon, "SymbolColon"},
        {LexicalKind::SymbolComma, "SymbolComma"},

        {LexicalKind::SymbolOpenBrace, "SymbolOpenBrace"},
        {LexicalKind::SymbolCloseBrace, "SymbolCloseBrace"},

        {LexicalKind::SymbolOpenParen, "SymbolOpenParen"},
        {LexicalKind::SymbolCloseParen, "SymbolCloseParen"},

        {LexicalKind::SymbolArrow, "SymbolArrow"},

        {LexicalKind::SymbolEquals, "SymbolEquals"},
        {LexicalKind::SymbolBooleanEquals, "SymbolBooleanEquals"},

        {LexicalKind::SymbolAdd, "SymbolAdd"},
        {LexicalKind::SymbolSubtract, "SymbolSubtract"},
        {LexicalKind::SymbolMultiply, "SymbolMultiply"},
        {LexicalKind::SymbolDivide, "SymbolDivide"},

    };

auto operator<<(std::ostream &stream, LexicalKind kind) -> std::ostream & {
  const auto x = lexical_kind_names_.find(kind);
  if (x == lexical_kind_names_.end()) {
    throw std::exception();
  }
  stream << x->second;
  return stream;
}

#pragma endregion

#pragma region Filters

constexpr auto should_match_whitespace(const char c) -> bool {
  return ('\0' < c and c <= ' ');
}

constexpr auto should_match_identifier(const char c) -> bool {
  return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z') or c == '_';
}

constexpr auto should_match_number(const char c) -> bool {
  return ('0' <= c and c <= '9');
}

constexpr auto should_match_symbol(const char c) -> bool {
  return ('!' <= c and c <= '/') or (':' <= c and c <= '@') or
         ('[' <= c and c <= '`') or ('{' <= c and c <= '~');
}

constexpr auto matches_whitespace(const char c) -> bool {
  return should_match_whitespace(c);
}

constexpr auto matches_identifier(const char c) -> bool {
  return should_match_identifier(c) or should_match_number(c);
}

constexpr auto matches_number(const char c) -> bool {
  return should_match_number(c);
}

constexpr auto matches_symbol(const char c) -> bool {
  return should_match_symbol(c);
}

#pragma endregion

#pragma region Lexer Global States

constexpr auto error_state = LexerState{
    [](LexerContext &ctx) -> LexerState { throw_not_implemented(); }};

constexpr auto exit_state = LexerState{};

extern const LexerState whitespace_start_state;
extern const LexerState identifier_start_state;
extern const LexerState number_start_state;
extern const LexerState symbol_start_state;

constexpr auto unknown_state = LexerState{[](LexerContext &ctx) -> LexerState {
  const auto &current = ctx.current();

  if (should_match_whitespace(current)) {
    return whitespace_start_state;
  }
  if (should_match_number(current)) {
    return number_start_state;
  }
  if (should_match_identifier(current)) {
    return identifier_start_state;
  }
  if (should_match_symbol(current)) {
    return symbol_start_state;
  }

  return error_state;
}};

constexpr auto start_state = LexerState{[](LexerContext &ctx) -> LexerState {
  return ctx.move_next_state(unknown_state, exit_state);
}};

#pragma endregion

#pragma region Lexer Whitespace States

constexpr LexerState whitespace_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      return ctx.move_next_state(matches_whitespace, whitespace_state,
                                 unknown_state, exit_state);
    }};

constexpr LexerState whitespace_start_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      // ctx.mark_start_of_token();
      return whitespace_state;
    }};

#pragma endregion

#pragma region Token Creation Helpers

constexpr auto create_empty_token(LexerContext &ctx, LexicalKind kind) {
  ctx.tokens.emplace_back(ctx.token_position(), kind);
}

constexpr auto create_value_token(LexerContext &ctx, LexicalKind kind) {
  ctx.tokens.emplace_back(ctx.token_position(), kind,
                          std::make_shared<std::string>(ctx.pop_buffer()));
}

#pragma endregion

#pragma region Lexer Identifier States

const auto keywords = std::unordered_map<std::string_view, LexicalKind>{
    {"var", LexicalKind::KeywordVar},
    {"func", LexicalKind::KeywordFunc},
    {"object", LexicalKind::KeywordObject},

    {"return", LexicalKind::KeywordReturn},

    {"if", LexicalKind::KeywordIf},
    {"elif", LexicalKind::KeywordElif},
    {"else", LexicalKind::KeywordElse},

    {"loop", LexicalKind::KeywordLoop},
    {"while", LexicalKind::KeywordWhile},
    {"for", LexicalKind::KeywordFor},
    {"foreach", LexicalKind::KeywordForeach},
};

auto identifier_cache = std::unordered_map<std::string, String>{};

auto cached_identifier(const std::string &str) -> String {
  auto &cache = identifier_cache;

  if (auto cached = cache.find(str); cached != cache.end()) {
    return cached->second;
  }

  return cache[str] = std::make_shared<std::string>(str);
}

auto create_identifier_token(LexerContext &ctx) -> void {
  const auto str = ctx.pop_buffer();
  const auto &kind = keywords.find(str);
  if (kind == keywords.end()) {
    ctx.tokens.emplace_back(ctx.token_position(), LexicalKind::Identifier,
                            cached_identifier(str));
  } else {
    create_empty_token(ctx, kind->second);
  }
}

constexpr auto identifier_end_exit_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      create_identifier_token(ctx);
      return exit_state;
    }};

constexpr auto identifier_end_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      create_identifier_token(ctx);
      return unknown_state;
    }};

constexpr LexerState identifier_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.buffer_current();
      return ctx.move_next_state(matches_identifier, identifier_state,
                                 identifier_end_state,
                                 identifier_end_exit_state);
    }};

constexpr LexerState identifier_start_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.mark_start_of_token();
      return identifier_state;
    }};

#pragma endregion

#pragma region Lexer Number States

constexpr auto create_number_token(LexerContext &ctx) -> void {
  create_value_token(ctx, LexicalKind::Number);
}

constexpr auto number_end_exit_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      create_number_token(ctx);
      return exit_state;
    }};

constexpr auto number_end_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      create_number_token(ctx);
      return unknown_state;
    }};

constexpr LexerState number_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.buffer_current();
      return ctx.move_next_state(matches_number, number_state, number_end_state,
                                 number_end_exit_state);
    }};

constexpr LexerState number_start_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.mark_start_of_token();
      return number_state;
    }};

#pragma endregion

#pragma region Lexer Symbol States

const auto symbols = std::unordered_map<std::string_view, LexicalKind>{
    {"", LexicalKind::Unknown},

    {";", LexicalKind::SymbolSemicolon},
    {":", LexicalKind::SymbolColon},
    {",", LexicalKind::SymbolComma},

    {"=", LexicalKind::SymbolEquals},
    {"==", LexicalKind::SymbolBooleanEquals},

    {"{", LexicalKind::SymbolOpenBrace},
    {"}", LexicalKind::SymbolCloseBrace},

    {"(", LexicalKind::SymbolOpenParen},
    {")", LexicalKind::SymbolCloseParen},

    {"->", LexicalKind::SymbolArrow},

    {"+", LexicalKind::SymbolAdd},
    {"-", LexicalKind::SymbolSubtract},
    {"*", LexicalKind::SymbolMultiply},
    {"/", LexicalKind::SymbolDivide},
};

auto get_symbol_token_kind(const std::string &str) -> LexicalKind {
  const auto &symbol = symbols.find(str);
  if (symbol == symbols.end()) {
    return LexicalKind::Unknown;
  }

  return symbol->second;
}

auto create_symbol_token(LexerContext &ctx) -> bool {
  auto str = ctx.pop_buffer();
  auto kind = get_symbol_token_kind(str);
  if (kind == LexicalKind::Unknown) {
    return false;
  }
  create_empty_token(ctx, kind);
  return true;
}

constexpr auto symbol_error_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      //
      throw_not_implemented();
    }};

constexpr auto symbol_end_exit_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      return create_symbol_token(ctx) ? exit_state : symbol_error_state;
    }};

constexpr auto symbol_end_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      return create_symbol_token(ctx) ? unknown_state : symbol_error_state;
    }};

constexpr LexerState symbol_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.buffer_current();
      return ctx.move_next_state(matches_symbol, symbol_state, symbol_end_state,
                                 symbol_end_exit_state);
    }};

constexpr LexerState symbol_start_state =
    LexerState{[](LexerContext &ctx) -> LexerState {
      ctx.mark_start_of_token();
      return symbol_state;
    }};

#pragma endregion

#pragma region File Position

auto operator<<(std::ostream &stream, const FilePosition &position)
    -> std::ostream & {
  stream << '(' << position.line() << ", " << position.col() << ')';
  return stream;
}

#pragma endregion

#pragma region Lexical Token

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

#pragma endregion

#pragma region Lexer Context

constexpr auto null_char = '\0';

LexerContext::LexerContext(const std::string &path)
    : stream_{path}, current_{null_char} {
  if (stream_.fail()) {
    throw std::exception("failed to open file.");
  }
}

auto LexerContext::move_next() -> bool {
  constexpr auto eof = std::ifstream::traits_type::eof();

  auto c = stream_.get();
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

Lexer::Lexer() : StateMachine<LexerContext>{start_state} {}

#pragma endregion