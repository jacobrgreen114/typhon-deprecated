// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "paths.hpp"

#include "project_config.hpp"
#include "lexical_analysis/lexer.hpp"
#include "syntax_analysis/parser.hpp"
#include "generation/generator.hpp"
#include "timer.hpp"

#ifdef TRACE
#define PARALLEL_COMPILATION false
#else
#define PARALLEL_COMPILATION true
#endif

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

auto find_source_files(const ProjectConfig& config) -> SourceCollection {
  if (!fs::exists(config.dir_source())) {
    std::cerr << "No source folder found" << std::endl;
    exit(-1);
  }

  auto sources = std::vector<SourceContext>{};

  for (auto& entry : fs::recursive_directory_iterator(config.dir_source())) {
    if (entry.is_regular_file() && entry.path().extension() == source_file_ext) {
      sources.emplace_back(entry.path());
    }
  }

  return sources;
}

// todo : implement already compiled optimization
auto parse_source(const SourceContext& source) -> std::shared_ptr<SyntaxTree> {
  TRACE_PRINT("Compiling : " << fs::absolute(source.path()) << std::endl);

  const auto rel_path = fs::relative(source.path(), src_dir_path).stem();

  auto tokens         = lex(source.path().string());
  write_tokens(rel_path, tokens);

  auto syntax = parse(tokens);
  write_syntax(rel_path, *syntax);

  //generate(rel_path, syntax);

  return syntax;
}

auto parse_sources(const SourceCollection& sources) -> SyntaxTreeCollection {
  auto trees = SyntaxTreeCollection{};
  trees.reserve(sources.size());

#if PARALLEL_COMPILATION
  auto compilation_futures = std::vector<std::future<std::shared_ptr<SyntaxTree>>>{};
  compilation_futures.reserve(sources.size());

  for (auto& source : sources) {
    compilation_futures.push_back(std::async(std::launch::async, compile, source));
  }

  for (auto& future : compilation_futures) {
    future.wait();
    trees.emplace_back(std::move(future.get()));
  }
#else
  for (auto& source : sources) {
    trees.emplace_back(parse_source(source));
  }
#endif

  return trees;
}

auto main(int argc, const char* argv[]) -> int {
  std::ios_base::sync_with_stdio(false);

  auto config = ProjectConfig::load();
  if (!config) {
    throw std::exception("Failed to load project config");
  }

  auto sources      = find_source_files(*config);
  auto syntax_trees = parse_sources(sources);

  return 0;
}
