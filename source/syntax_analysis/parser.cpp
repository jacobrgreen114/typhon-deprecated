
// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "parser.hpp"

#include "parser_statemachine.hpp"

auto parse(const TokenCollection& tokens) -> std::shared_ptr<SourceNode> {
  auto parser = Parser{};
  auto context = ParserContext{tokens};
  parser.run(context);
  return context.source;
}