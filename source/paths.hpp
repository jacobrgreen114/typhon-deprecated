// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

constexpr auto source_file_ext  = std::string_view{".ty"};
constexpr auto gen_src_file_ext = std::string_view{".cpp"};

constexpr auto src_dir_name     = std::string_view{"source"};
const auto src_dir_path         = fs::proximate(src_dir_name);

constexpr auto obj_dir_name     = std::string_view{"obj"};
const auto obj_dir_path         = fs::proximate(obj_dir_name);

constexpr auto gen_dir_name     = std::string_view{"gen"};
const auto gen_dir_path         = obj_dir_path / gen_dir_name;

constexpr auto bin_dir_name     = std::string_view{"bin"};
const auto bin_dir_path         = fs::proximate(bin_dir_name);

auto rel_src_path_to_gen_src_file_path(const fs::path& rel_path) -> fs::path;

#ifdef TRACE
constexpr auto tok_file_ext = std::string_view{".tok.xml"};
constexpr auto syn_file_ext = std::string_view{".syn.xml"};

constexpr auto trc_dir_name = std::string_view{"trace"};
constexpr auto tok_dir_name = std::string_view{"tokens"};
constexpr auto syn_dir_name = std::string_view{"syntax"};

const auto tok_dir_path     = obj_dir_path / trc_dir_name;
const auto syn_dir_path     = obj_dir_path / trc_dir_name;
#endif