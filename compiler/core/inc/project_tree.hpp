// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "syntax_tree.hpp"

class NameSpace final {
 public:
  using SubSpace = std::shared_ptr<NameSpace>;

 private:
  String name_;
  std::vector<SubSpace> sub_spaces_;
  std::vector<std::shared_ptr<SyntaxTree>> syntax_trees_;

 public:
  explicit NameSpace(const String& name)
      : name_{name} {}

  NODISCARD auto& name() const { return name_; }
  NODISCARD auto& sub_spaces() const { return sub_spaces_; }
  NODISCARD auto& trees() const { return syntax_trees_; }

  auto push_sub_space(const SubSpace& ns) { sub_spaces_.emplace_back(ns); }
  auto push_tree(const std::shared_ptr<SyntaxTree>& tree) { syntax_trees_.emplace_back(tree); }
};

class ProjectTree final {
  std::shared_ptr<NameSpace> root_;

 public:
  ProjectTree()
      : root_{std::make_shared<NameSpace>(nullptr)} {};

  NODISCARD auto& root() const { return root_; }
};