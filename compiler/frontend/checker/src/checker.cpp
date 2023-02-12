// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "checker.hpp"
#include "timer.hpp"

auto place_tree(const ProjectTree& project, std::unique_ptr<SyntaxTree> tree) {
  auto current_namespace = project.root().get();

  // If syntax trees doesn't specify a namespace place at root namespace
  auto tree_namespace    = tree->get_namespace();
  if (!tree_namespace) {
    current_namespace->push_tree(std::move(tree));
    return;
  }

  for (auto& ns : tree_namespace->namespaces()) {
    auto& sub_spaces = current_namespace->sub_spaces();

    // Find namespace in project tree
    auto desired_ns = std::find_if(sub_spaces.begin(), sub_spaces.end(), [=](auto& sub_ns) -> bool {
      return sub_ns->name() == ns;
    });

    if (desired_ns != sub_spaces.end()) {
      current_namespace = desired_ns->get();
    } else {
      auto new_space = std::make_unique<NameSpace>(ns);
      auto* temp     = new_space.get();
      current_namespace->push_sub_space(std::move(new_space));
      current_namespace = temp;
    }

    current_namespace->push_tree(std::move(tree));
  }
}

auto place_syntax_trees_into_project(const ProjectTree& project,
                                     SyntaxTreeCollection& syntax_trees) {
  for (auto& tree : syntax_trees) {
    place_tree(project, std::move(tree));
  }
}

auto check(SyntaxTreeCollection& syntax_trees) -> ProjectTree {
  TRACE_TIMER("checker");
  auto project_tree = ProjectTree{};
  place_syntax_trees_into_project(project_tree, syntax_trees);

  return project_tree;
}