// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include <filesystem>
#include <iostream>
#include <span>

#include "lexical_analysis.hpp"
#include "syntax_analysis.hpp"

auto lex(const std::string& path) -> TokenCollection {
  auto lexer   = Lexer{};
  auto context = LexerContext{path};
  lexer.run(context);
  return context.tokens;
}

auto parse(const TokenCollection& tokens) {
  auto parser  = Parser{};
  auto context = ParserContext{tokens};
  parser.run(context);
}

auto main(int argc, const char* argv[]) -> int {
#ifdef _DEBUG
  const auto cwd = std::filesystem::current_path();
  std::cout << "cwd : " << cwd << std::endl;

  const auto args = std::span<const char*>{argv, static_cast<size_t>(argc)};
  for (auto& arg : args) {
    std::cout << arg << std::endl;
  }
#endif

  auto tokens = lex("./source/main.ty");

#ifdef _DEBUG
  for (auto& token : tokens) {
    std::cout << token << std::endl;
  }
#endif

  parse(tokens);

  return 0;
}
