// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "paths.hpp"

#include "project_config.hpp"
#include "lexical_analysis/lexer.hpp"
#include "syntax_analysis/parser.hpp"
#include "generation/generator.hpp"
#include "timer.hpp"

auto write_tokens(const fs::path& rel_path, const TokenCollection& tokens) -> void {
#ifdef TRACE
  const auto token_path = (tok_dir_path / rel_path).concat(tok_file_ext);
  fs::create_directories(token_path.parent_path());

  auto token_file = std::ofstream{token_path, std::ofstream::out | std::ofstream::trunc};
  for (auto& token : tokens) {
    token.serialize(token_file, "token");
  }
#endif
}

auto write_syntax(const fs::path& rel_path, const SyntaxTree& source) -> void {
#ifdef TRACE
  const auto syntax_path = (syn_dir_path / rel_path).concat(syn_file_ext);
  fs::create_directories(syntax_path.parent_path());

  auto syntax_file = std::ofstream{syntax_path, std::ofstream::out | std::ofstream::trunc};
  source.serialize(syntax_file, "Source");
#endif
}

// todo : implement already compiled optimization
auto compile(const fs::path& file_path) -> void {
  std::cout << "Compiling : " << fs::absolute(file_path) << std::endl;

  const auto rel_path = fs::relative(file_path, src_dir_path).stem();

  // TRACE_TIMER("total compilation")

  auto tokens         = lex(file_path.string());
  write_tokens(rel_path, tokens);

  auto source = parse(tokens);
  write_syntax(rel_path, *source);

  generate(rel_path, source);
}

auto main(int argc, const char* argv[]) -> int {
  auto config = ProjectConfig::load();
  if (!config) {
    throw std::exception("Failed to load project config");
  }

  if (!fs::exists(config->dir_source())) {
    std::cerr << "No source folder found" << std::endl;
    return -1;
  }

  // todo : parallelize compilation
  for (auto& entry : fs::recursive_directory_iterator(config->dir_source())) {
    if (entry.is_regular_file() && entry.path().extension() == source_file_ext) {
      compile(entry.path());
    }
  }

  return 0;
}
