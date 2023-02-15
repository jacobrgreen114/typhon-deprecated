
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
  static constexpr auto gen_dir_name     = std::string_view{"gen"};
  static constexpr auto gen_src_dir_name = std::string_view{"src"};

#ifdef TRACE
  static constexpr auto trace_dir_name = std::string_view{"trace"};
#endif

  BinaryType binary_type_ = BinaryType::Exe;
  bool link_std_          = true;

  std::string name_;
  fs::path source_dir_ = fs::proximate("src");
  fs::path obj_dir_    = fs::proximate("obj");
  fs::path bin_dir_    = fs::proximate("bin");

  fs::path gen_dir_    = obj_dir_ / gen_dir_name;
  fs::path gen_src_    = gen_dir_ / gen_src_dir_name;

#ifdef TRACE
  fs::path trace_dir_ = obj_dir_ / trace_dir_name;
#endif

  std::vector<ProjectConfiguration::ConstPointer> configurations_;
  std::vector<ProjectReference::ConstPointer> references_;

 public:
  NODISCARD auto binary_type() const { return binary_type_; }
  NODISCARD auto link_std() const { return link_std_; }

  NODISCARD auto& name() const { return name_; }

  NODISCARD auto& dir_source() const { return source_dir_; }
  NODISCARD auto& dir_build() const { return obj_dir_; }
  NODISCARD auto& dir_binary() const { return bin_dir_; }

  //NODISCARD auto& dir_gen() const { return gen_dir_; }
  NODISCARD auto& dir_gen_source() const { return gen_src_; }

#ifdef TRACE
  NODISCARD auto& dir_trace() const { return trace_dir_; }
#endif

  auto set_name(const std::string_view name) { name_ = name; }
  auto set_source_dir(const std::string_view path) { source_dir_ = path; }
  auto set_build_dir(const std::string_view path) {
    obj_dir_ = path;
    //gen_dir_     = obj_dir_ / gen_dir_name;
    gen_src_ = obj_dir_ / gen_src_dir_name;
#ifdef TRACE
    trace_dir_ = obj_dir_ / trace_dir_name;
#endif
  }
  auto set_binary_dir(const std::string_view path) { bin_dir_ = path; }

  auto set_binary_type(BinaryType type) { binary_type_ = type; }
  auto set_link_std(bool link_std) { link_std_ = link_std; }

  static auto load(const fs::path& dir_path = fs::current_path()) -> ConstPointer;
};
