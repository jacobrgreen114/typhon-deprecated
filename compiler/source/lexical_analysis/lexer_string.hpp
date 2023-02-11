// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "lexer_sm.hpp"

constexpr auto should_match_string(char c) -> bool { return c == '"'; }

extern const LexerState string_start_state;
