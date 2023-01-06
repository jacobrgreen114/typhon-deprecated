// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include <filesystem>
#include <iostream>
#include <span>

#include "lexical_analysis.hpp"

auto main(int argc, const char* argv[]) -> int {
#ifdef _DEBUG
  const auto cwd = std::filesystem::current_path();
  std::cout << "cwd : " << cwd << std::endl;

  const auto args = std::span<const char*>{argv, static_cast<size_t>(argc)};
  for (auto& arg : args) {
    std::cout << arg << std::endl;
  }
#endif
  
  auto lexer = Lexer{};
  auto context = LexerContext{"./source/main.ty"};
  lexer.run(context);

#ifdef _DEBUG
  for (auto& token : context.tokens) {
    std::cout << token << std::endl;
  }
#endif

  return 0;
}
