// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "lexical_analysis/lexer.hpp"
#include "syntax_analysis/parser.hpp"

auto main(int argc, const char* argv[]) -> int {
  const auto args = std::span<const char*>{argv, static_cast<size_t>(argc)};

#ifdef TRACE
  const auto cwd = std::filesystem::current_path();
  std::cout << "cwd : " << cwd << std::endl;

  for (auto& arg : args) {
    std::cout << arg << std::endl;
  }
#endif

  auto tokens = lex("./source/main.ty");

#ifdef TRACE
  for (auto& token : tokens) {
    std::cout << token << std::endl;
  }
#endif

  auto source = parse(tokens);

#ifdef TRACE
  {
    auto syntax_file = std::ofstream{"./obj/main.syn.xml",
                                     std::ofstream::out | std::ofstream::trunc};
    source->serialize(syntax_file, "Source");
  }
#endif

  return 0;
}
