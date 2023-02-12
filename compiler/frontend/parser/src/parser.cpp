
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser.hpp"

#include "parser_sm.hpp"

#include "timer.hpp"

auto parse(const TokenCollection& tokens) -> std::unique_ptr<SyntaxTree> {
  auto parser  = Parser{};
  auto context = ParserContext{tokens};

  TRACE_TIMER("parse");
  parser.run(context);
  return std::move(context.source);
}