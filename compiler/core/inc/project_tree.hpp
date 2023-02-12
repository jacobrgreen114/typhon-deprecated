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

  auto push_sub_space(SubSpace ns) { sub_spaces_.emplace_back(std::move(ns)); }
  auto push_tree(std::unique_ptr<SyntaxTree> tree) { syntax_trees_.emplace_back(std::move(tree)); }
};

class ProjectTree final {
  std::unique_ptr<NameSpace> root_;

 public:
  ProjectTree()
      : root_{std::make_unique<NameSpace>()} {};

  NODISCARD auto& root() const { return root_; }
};