// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "common.hpp"

#include "paths.hpp"

class SourceContext final {
  fs::path source_dir_;
  fs::path path_;
  fs::path rel_path_;

  fs::path gen_source_path_;
  fs::path gen_header_internal_path_;
  fs::path gen_header_public_path_;

 public:
  explicit SourceContext(const fs::path& source_dir, const fs::path& file_path)
      : source_dir_{source_dir},
        path_{file_path},
        rel_path_{fs::relative(file_path, source_dir)},
        gen_source_path_{(gen_dir_src_path / rel_path_).concat(gen_src_file_ext)},
        gen_header_internal_path_{(gen_dir_src_path / rel_path_).concat(gen_hdr_file_ext)},
        gen_header_public_path_{(gen_dir_inc_path / rel_path_).concat(gen_hdr_file_ext)} {}

  NODISCARD auto& path() const { return path_; }

  NODISCARD auto absolute_path() const { return fs::absolute(path()); }

  NODISCARD auto& rel_path() const { return rel_path_; }

  NODISCARD auto& gen_source_path() const { return gen_source_path_; }
  NODISCARD auto& gen_header_internal_path() const { return gen_header_internal_path_; }
  NODISCARD auto& gen_header_public_path() const { return gen_header_public_path_; }

  NODISCARD auto filename() const { return path_.filename(); }
};

using SourceCollection = std::vector<std::shared_ptr<SourceContext>>;
