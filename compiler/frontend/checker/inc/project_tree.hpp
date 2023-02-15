// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include <utility>

#include "syntax_tree.hpp"

class NameSpace final {
 public:
  using SubSpace = std::unique_ptr<NameSpace>;

 private:
  NameSpace* parent_ = nullptr;
  std::string name_;
  std::vector<SubSpace> sub_spaces_;
  std::vector<std::unique_ptr<SyntaxTree>> syntax_trees_;

 public:
  explicit NameSpace() = default;

  explicit NameSpace(std::string name)
      : name_{std::move(name)} {}

  NODISCARD auto& name() const { return name_; }
  NODISCARD auto& sub_spaces() const { return sub_spaces_; }
  NODISCARD auto& sub_spaces() { return sub_spaces_; }
  NODISCARD auto& trees() const { return syntax_trees_; }

  NODISCARD auto parent() const { return parent_; }

 private:
  NODISCARD auto full_name_(const std::string_view sep) const {
    auto full_name = name();
    for (auto* par = parent(); par != nullptr; par = par->parent()) {
      if (!par->name().empty()) {
        full_name.insert(0, sep);
        full_name.insert(0, par->name());
      }
    }
    return full_name;
  }

 public:
  NODISCARD auto full_name() const { return full_name_("::"); }
  NODISCARD auto file_name() const { return full_name_("."); }

  NODISCARD auto gen_header_path() const {
    return gen_dir_src_path / file_name().append(".ty").append(gen_hdr_file_ext);
  }

  auto push_sub_space(SubSpace ns) {
    ns->parent_ = this;
    sub_spaces_.emplace_back(std::move(ns));
  }

  auto push_tree(std::unique_ptr<SyntaxTree> tree) { syntax_trees_.emplace_back(std::move(tree)); }
};

class ProjectTree final {
  std::unique_ptr<NameSpace> root_;

 public:
  ProjectTree()
      : root_{std::make_unique<NameSpace>()} {};

  NODISCARD auto& root() const { return root_; }
};