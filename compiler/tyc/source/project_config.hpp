
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "paths.hpp"

enum class BinaryType {
  Exe,
  Lib,
  Dyn
};

class ProjectConfig {
  BinaryType binary_type_ = BinaryType::Exe;

  std::string name_project_;

  fs::path source_dir_ = src_dir_path;
  fs::path obj_dir_    = obj_dir_path;
  fs::path bin_dir_    = bin_dir_path;

 public:
  NODISCARD auto binary_type() const { return binary_type_; }

  NODISCARD auto& name_project() const { return name_project_; }

  NODISCARD auto& dir_source() const { return source_dir_; }
  NODISCARD auto& dir_obj() const { return obj_dir_; }
  NODISCARD auto& dir_bin() const { return bin_dir_; }

  static std::unique_ptr<ProjectConfig> load(const fs::path& dir_path = fs::current_path());
};
