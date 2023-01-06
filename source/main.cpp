// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "lexical_analysis.hpp"

auto main() -> int {
  auto lexer = Lexer{};
  auto context = LexerContext{"source/main.ty"};
  lexer.run(context);

  return 0;
}
