// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexical_analysis/lexer.hpp"
#include "syntax_analysis/parser.hpp"

constexpr auto src_dir_name = std::string_view{"source"};
constexpr auto obj_dir_name = std::string_view{"obj"};
constexpr auto bin_dir_name = std::string_view{"bin"};

constexpr auto source_file_ext = std::string_view{".ty"};
constexpr auto tok_file_ext    = std::string_view{".tok.xml"};
constexpr auto syn_file_ext    = std::string_view{".syn.xml"};

const auto src_dir_path = fs::proximate(src_dir_name);
const auto obj_dir_path = fs::proximate(obj_dir_name);
const auto bin_dir_path = fs::proximate(bin_dir_name);

#ifdef TRACE
constexpr auto trc_dir_name = std::string_view{"trace"};
constexpr auto tok_dir_name = std::string_view{"tokens"};
constexpr auto syn_dir_name = std::string_view{"syntax"};
const auto tok_dir_path     = obj_dir_path / trc_dir_name;
const auto syn_dir_path     = obj_dir_path / trc_dir_name;
#endif

auto write_tokens(const fs::path& source_path, const TokenCollection& tokens) -> void {
#ifdef TRACE
  const auto rel_path   = fs::relative(source_path, src_dir_path);
  const auto token_path = tok_dir_path / (rel_path.stem().concat(tok_file_ext));
  fs::create_directories(tok_dir_path);

  auto token_file = std::ofstream{token_path, std::ofstream::out | std::ofstream::trunc};
  for (auto& token : tokens) {
    token.serialize(token_file, "token");
  }
#endif
}

auto write_syntax(const fs::path& source_path, const SourceNode& source) -> void {
#ifdef TRACE
  const auto rel_path    = fs::relative(source_path, src_dir_path);
  const auto syntax_path = syn_dir_path / (rel_path.stem().concat(syn_file_ext));
  fs::create_directories(syn_dir_path);

  auto syntax_file = std::ofstream{syntax_path, std::ofstream::out | std::ofstream::trunc};
  source.serialize(syntax_file, "Source");
#endif
}

auto compile(const fs::path& file_path) -> void {
  std::cout << "Compiling : " << fs::absolute(file_path) << std::endl;
  auto tokens = lex(file_path.string());
  write_tokens(file_path, tokens);

  auto source = parse(tokens);
  write_syntax(file_path, *source);
}

auto main(int argc, const char* argv[]) -> int {
  if (!fs::exists(src_dir_path)) {
    std::cerr << "No source folder found" << std::endl;
    return -1;
  }

  for (auto& entry : fs::recursive_directory_iterator(src_dir_path)) {
    if (entry.is_regular_file() && entry.path().extension() == source_file_ext) {
      compile(entry.path());
    }
  }

  return 0;
}
