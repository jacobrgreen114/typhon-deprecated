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

 public:
  explicit NameSpace(const String& name)
      : name_{name} {}

  NODISCARD auto& name() const { return name_; }
  NODISCARD auto& sub_spaces() const { return sub_spaces_; }

  auto push_sub_space(const SubSpace& ns) -> void { sub_spaces_.emplace_back(ns); }
};

class ProjectTree final {
  NameSpace root_;

 public:
  NODISCARD auto root() const { return &root_; }
};