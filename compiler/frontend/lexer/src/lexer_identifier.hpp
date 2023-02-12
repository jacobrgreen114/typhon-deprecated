// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "lexer_sm.hpp"

extern const LexerState identifier_start_state;

constexpr auto should_match_identifier(const char c) -> bool {
  return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z') or c == '_';
}

constexpr auto matches_identifier(const char c) -> bool {
  return should_match_identifier(c) or ('0' <= c and c <= '9');
}
