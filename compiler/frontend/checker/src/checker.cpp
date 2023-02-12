// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "checker.hpp"
#include "timer.hpp"

auto place_tree(const std::shared_ptr<ProjectTree>& project,
                const std::shared_ptr<SyntaxTree>& tree) {
  auto current_namespace = project->root();

  // If syntax trees doesn't specify a namespace place at root namespace
  auto tree_namespace    = tree->get_namespace();
  if (!tree_namespace) {
    current_namespace->push_tree(tree);
    return;
  }

  for (auto& ns : tree_namespace->namespaces()) {
    auto& sub_spaces = current_namespace->sub_spaces();

    // Find namespace in project tree
    auto desired_ns = std::find_if(sub_spaces.begin(), sub_spaces.end(), [=](auto& sub_ns) -> bool {
      return *sub_ns->name() == *ns;
    });

    if (desired_ns != sub_spaces.end()) {
      current_namespace = *desired_ns;
    } else {
      auto new_space = std::make_shared<NameSpace>(ns);
      current_namespace->push_sub_space(new_space);
      current_namespace = new_space;
    }

    current_namespace->push_tree(tree);
  }
}

auto place_syntax_trees_into_project(const std::shared_ptr<ProjectTree>& project,
                                     const SyntaxTreeCollection& syntax_trees) {
  for (auto& tree : syntax_trees) {
    place_tree(project, tree);
  }
}

auto check(const SyntaxTreeCollection& syntax_trees) -> std::shared_ptr<ProjectTree> {
  TRACE_TIMER("checker");
  auto project_tree = std::make_shared<ProjectTree>();
  place_syntax_trees_into_project(project_tree, syntax_trees);

  return project_tree;
}