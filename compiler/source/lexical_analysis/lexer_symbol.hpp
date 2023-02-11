// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "lexer_sm.hpp"

extern const LexerState symbol_start_state;

constexpr auto should_match_symbol(const char c) -> bool {
  return ('!' <= c and c <= '/') or (':' <= c and c <= '@') or ('[' <= c and c <= '`') or
         ('{' <= c and c <= '~');
}

constexpr auto matches_symbol(const char c) -> bool { return should_match_symbol(c); }