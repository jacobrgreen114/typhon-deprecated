// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "common.hpp"

constexpr auto source_file_ext  = std::string_view{".ty"};
constexpr auto project_file_ext = std::string_view{".typroj"};
//constexpr auto solution_file_ext = std::string_view{".tysln"};

constexpr auto gen_src_file_ext = std::string_view{".cpp"};
constexpr auto gen_hdr_file_ext = std::string_view{".hpp"};

// constexpr auto src_dir_name     = std::string_view{"src"};
// const auto src_dir_path         = fs::proximate(src_dir_name);
//
// constexpr auto obj_dir_name     = std::string_view{"obj"};
// const auto obj_dir_path         = fs::proximate(obj_dir_name);
//
// constexpr auto gen_dir_name     = std::string_view{"gen"};
// const auto gen_dir_path         = obj_dir_path / gen_dir_name;

// constexpr auto gen_src_name     = std::string_view{"src"};
// const auto gen_dir_src_path     = gen_dir_path / gen_src_name;
//
// constexpr auto gen_inc_name     = std::string_view{"inc"};
// const auto gen_dir_inc_path     = gen_dir_path / gen_inc_name;

// constexpr auto bin_dir_name     = std::string_view{"bin"};
// const auto bin_dir_path         = fs::proximate(bin_dir_name);

#ifdef TRACE
constexpr auto tok_file_ext = std::string_view{".tok.xml"};
constexpr auto syn_file_ext = std::string_view{".syn.xml"};
//
// constexpr auto trc_dir_name = std::string_view{"_trace"};
//
// const auto tok_dir_path     = obj_dir_path / trc_dir_name;
// const auto syn_dir_path     = obj_dir_path / trc_dir_name;
#endif