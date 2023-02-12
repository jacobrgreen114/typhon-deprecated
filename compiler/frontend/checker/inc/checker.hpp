// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "project_tree.hpp"

auto check(const SyntaxTreeCollection& syntax_trees) -> std::shared_ptr<ProjectTree>;