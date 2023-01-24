// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "lexer_identifier.hpp"

const auto keywords = std::unordered_map<std::string_view, LexicalKind>{
    {"var", LexicalKind::KeywordVar},
    {"func", LexicalKind::KeywordFunc},
    {"object", LexicalKind::KeywordObject},

    {"return", LexicalKind::KeywordReturn},

    {"if", LexicalKind::KeywordIf},
    {"elif", LexicalKind::KeywordElif},
    {"else", LexicalKind::KeywordElse},

    {"switch", LexicalKind::KeywordSwitch},
    {"match", LexicalKind::KeywordMatch},

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