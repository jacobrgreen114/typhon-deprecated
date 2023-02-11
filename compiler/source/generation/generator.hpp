// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "../syntax_analysis/syntax_tree.hpp"

auto generate(const fs::path& file_path, const std::shared_ptr<SyntaxTree>& source) -> void;