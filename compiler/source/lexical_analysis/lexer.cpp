// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer.hpp"

#include "lexer_sm.hpp"

#include "../timer.hpp"

auto lex(const std::shared_ptr<SourceContext>& source) -> const TokenCollection {
  auto lexer   = Lexer{};
  auto context = LexerContext{source->path()};

  TRACE_TIMER("lex")
  lexer.run(context);
  return TokenCollection{source, context.tokens};
}