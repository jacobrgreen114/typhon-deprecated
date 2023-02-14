// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "gen_common.hpp"

auto write_forward_decl(std::ostream& writer, const VariableDefinition& def) -> void;

auto write_def(std::ostream& writer, const VariableDefinition& def) -> void;