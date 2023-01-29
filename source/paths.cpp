// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "paths.hpp"

auto rel_src_path_to_gen_src_file_path(const fs::path& rel_path) -> fs::path {
  return (gen_dir_path / rel_path).concat(gen_src_file_ext);;
}
