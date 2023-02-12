// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "gen_common.hpp"

auto write_block(std::ostream& writer, const std::shared_ptr<StatementBlock>& body) -> void;