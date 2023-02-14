// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer.hpp"

#include "lexer_sm.hpp"

#include "timer.hpp"

auto lex(SourceContext::Pointer source) -> const TokenCollection {
  auto lexer   = Lexer{};
  auto context = LexerContext{source->path()};

  {
    TRACE_TIMER("Lexer");
    lexer.run(context);
  }

  return TokenCollection{std::move(source), std::move(context.tokens)};
}