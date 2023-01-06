// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#ifdef __cplusplus

#pragma once

#include "lexical_analysis.hpp"
#include "state_machine.hpp"

using LexicalTokenPredicate = Predicate<const LexicalToken&>;

class ParserContext;
using ParserState = State<ParserContext>;

class ParserContext final
    : public EnumeratingContext<ParserContext, LexicalToken> {
  const std::vector<LexicalToken> tokens_;
  bool started_;
  std::vector<LexicalToken>::const_iterator current_;

 public:
  explicit ParserContext(const std::vector<LexicalToken>& tokens);
  virtual ~ParserContext() = default;

  auto current() -> const LexicalToken& override;
  auto move_next() -> bool override;
};

class Parser final : public StateMachine<ParserContext> {
 public:
  Parser();
};

#else
#error "!cplusplus"
#endif