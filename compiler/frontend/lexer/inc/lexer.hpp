// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "token.hpp"

auto lex(const std::shared_ptr<SourceContext>& source) -> const TokenCollection;