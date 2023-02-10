// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer.hpp"

#include "lexer_sm.hpp"

#include "../timer.hpp"

auto lex(const std::string& path) -> TokenCollection {
  auto lexer   = Lexer{};
  auto context = LexerContext{path};

  TRACE_TIMER("lex")
  lexer.run(context);
  return context.tokens;
}