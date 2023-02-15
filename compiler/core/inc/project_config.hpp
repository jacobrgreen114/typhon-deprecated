
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "paths.hpp"

enum class BinaryType : uint8_t {
  Exe,
  Lib,
  Dyn
};

class ProjectConfiguration final {
 public:
  using Pointer      = std::unique_ptr<ProjectConfiguration>;
  using ConstPointer = std::unique_ptr<const ProjectConfiguration>;

 private:
};

class ProjectReference final {
 public:
  using Pointer      = std::unique_ptr<ProjectConfiguration>;
  using ConstPointer = std::unique_ptr<const ProjectConfiguration>;

 private:
};

class ProjectConfig {
 public:
  using Pointer      = std::unique_ptr<ProjectConfig>;
  using ConstPointer = std::unique_ptr<const ProjectConfig>;

 private:
  BinaryType binary_type_ = BinaryType::Exe;
  bool link_std_          = true;

  std::string name_;
  fs::path source_dir_ = src_dir_path;
  fs::path obj_dir_    = obj_dir_path;
  fs::path bin_dir_    = bin_dir_path;

  std::vector<ProjectConfiguration::ConstPointer> configurations_;
  std::vector<ProjectReference::ConstPointer> references_;

 public:
  NODISCARD auto binary_type() const { return binary_type_; }

  NODISCARD auto& name() const { return name_; }

  NODISCARD auto& dir_source() const { return source_dir_; }
  NODISCARD auto& dir_build() const { return obj_dir_; }
  NODISCARD auto& dir_binary() const { return bin_dir_; }

  NODISCARD auto link_std() const { return link_std_; }

  auto set_name(const std::string_view name) { name_ = name; }
  auto set_source_dir(const std::string_view path) { source_dir_ = path; }
  auto set_build_dir(const std::string_view path) { obj_dir_ = path; }
  auto set_binary_dir(const std::string_view path) { bin_dir_ = path; }

  auto set_binary_type(BinaryType type) { binary_type_ = type; }
  auto set_link_std(bool link_std) { link_std_ = link_std; }

  static auto load(const fs::path& dir_path = fs::current_path()) -> ConstPointer;
};
