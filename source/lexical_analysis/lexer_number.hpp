// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "lexer_source.hpp"

extern const LexerState number_start_state;

constexpr auto should_match_number(const char c) -> bool {
  return ('0' <= c and c <= '9');
}

constexpr auto matches_number(const char c) -> bool {
  return should_match_number(c);
}