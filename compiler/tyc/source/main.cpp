// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "paths.hpp"

#include "project_config.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "checker.hpp"
#include "generator.hpp"
#include "timer.hpp"

#ifdef TRACE
#define PARALLEL_COMPILATION false
#else
#define PARALLEL_COMPILATION true
#endif

auto write_tokens(const SourceContext& source, const TokenCollection& tokens) -> void {
#ifdef TRACE
  const auto& token_path = source.gen_token_path();
  fs::create_directories(token_path.parent_path());

  auto token_file = std::ofstream{token_path};
  for (auto& token : tokens.tokens()) {
    token.serialize(token_file, "token");
  }
#endif
}

auto write_syntax(const SourceContext& source, const SyntaxTree& tree) -> void {
#ifdef TRACE
  const auto& syntax_path = source.gen_syntax_path();
  fs::create_directories(syntax_path.parent_path());

  auto syntax_file = std::ofstream{syntax_path};
  tree.serialize(syntax_file, "Source");
#endif
}

auto find_source_files(const ProjectConfig& config) -> SourceCollection {
  if (!fs::exists(config.dir_source())) {
    std::cerr << "No source folder found" << std::endl;
    exit(-1);
  }

  auto sources = SourceCollection{};

  for (auto& entry : fs::recursive_directory_iterator(config.dir_source())) {
    if (entry.is_regular_file() && entry.path().extension() == source_file_ext) {
      auto& path = entry.path();
      sources.emplace_back(std::make_unique<SourceContext>(config, path));
    }
  }

  return sources;
}

// todo : implement already compiled optimization
auto parse_source(const SourceContext::Pointer& psource) -> std::unique_ptr<SyntaxTree> {
  auto& source = deref(psource);
  TRACE_PRINT("Compiling : " << source.absolute_path() << std::endl);

  auto tokens = lex(psource);
  write_tokens(source, tokens);

  auto syntax = parse(tokens);
  write_syntax(source, *syntax);

  return syntax;
}

auto parse_sources(const SourceCollection& sources) -> SyntaxTreeCollection {
  auto trees = SyntaxTreeCollection{};
  trees.reserve(sources.size());

#if PARALLEL_COMPILATION
  auto compilation_futures = std::vector<std::future<std::unique_ptr<SyntaxTree>>>{};
  compilation_futures.reserve(sources.size());

  for (auto& source : sources) {
    compilation_futures.push_back(std::async(std::launch::async, parse_source, source));
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

class Compiler final {
  ProjectConfig::ConstPointer config_;

 public:
  Compiler()
      : config_{ProjectConfig::load()} {
    if (!config_) {
      throw std::exception("Failed to load project config");
    }
  }

  auto run() -> int {
    auto& config = deref(config_);

    auto sources      = find_source_files(config);
    auto syntax_trees = parse_sources(sources);
    auto project_tree = check(syntax_trees);

    generate(config, project_tree);
    return 0;
  }
};

auto main(int argc, const char* argv[]) -> int {
  std::ios_base::sync_with_stdio(false);

  auto app     = Compiler{};

  auto timer   = Timer{};
  auto result  = app.run();
  auto elapsed = timer.elapsed();

  std::cout << "Compilation Time : " << std::fixed << elapsed << " secs" << std::endl;
  return result;
}
