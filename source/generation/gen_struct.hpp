// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "gen_common.hpp"

auto write_forward_decl(std::ostream& writer, const std::shared_ptr<StructDefinition>& def) -> void;

auto write_struct_definition(std::ostream& writer, const std::shared_ptr<StructDefinition>& str)
    -> void;