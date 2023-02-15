// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "common.hpp"
#include "paths.hpp"
#include "project_config.hpp"

class SourceContext final {
 public:
  using Pointer = std::shared_ptr<SourceContext>;

 private:
  fs::path path_;
  fs::path rel_path_;

  fs::path gen_source_path_;
  fs::path gen_header_internal_path_;
  fs::path gen_header_public_path_;

#ifdef TRACE
  fs::path gen_token_path_;
  fs::path gen_syntax_path_;
#endif

 public:
  explicit SourceContext(const ProjectConfig& config, const fs::path& file_path)
      : path_{file_path},
        rel_path_{fs::relative(file_path, config.dir_source())},
        gen_source_path_{config.dir_gen_source() / rel_path_.stem() += gen_src_file_ext},
        gen_header_internal_path_{config.dir_gen_source() / rel_path_.stem() += gen_hdr_file_ext},
        gen_header_public_path_{config.dir_gen_source() / rel_path_.stem() += gen_hdr_file_ext}
#ifdef TRACE
        ,
        gen_token_path_{config.dir_trace() / rel_path_ += tok_file_ext},
        gen_syntax_path_{config.dir_trace() / rel_path_ += syn_file_ext}
#endif
  {
  }

  NODISCARD auto& path() const { return path_; }

  NODISCARD auto absolute_path() const { return fs::absolute(path()); }

  NODISCARD auto& rel_path() const { return rel_path_; }

  NODISCARD auto& gen_source_path() const { return gen_source_path_; }
  NODISCARD auto& gen_header_internal_path() const { return gen_header_internal_path_; }
  NODISCARD auto& gen_header_public_path() const { return gen_header_public_path_; }

  NODISCARD auto filename() const { return path_.filename(); }

#ifdef TRACE
  NODISCARD auto& gen_token_path() const { return gen_token_path_; }
  NODISCARD auto& gen_syntax_path() const { return gen_syntax_path_; }
#endif
};

using SourceCollection = std::vector<std::shared_ptr<SourceContext>>;
