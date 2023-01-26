// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif  //__cplusplus

#include "syntax.hpp"

auto parse(const TokenCollection& tokens) -> std::shared_ptr<SourceNode>;