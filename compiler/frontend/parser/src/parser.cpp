
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser.hpp"

#include "parser_statemachine.hpp"

#include "timer.hpp"

auto parse(const TokenCollection& tokens) -> std::shared_ptr<SyntaxTree> {
  auto parser  = Parser{};
  auto context = ParserContext{tokens};

  TRACE_TIMER("parse");
  parser.run(context);
  return context.source;
}