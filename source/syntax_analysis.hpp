// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#ifdef __cplusplus

#pragma once

#include <stack>

#include "lexical_analysis.hpp"
#include "state_machine.hpp"

enum class SyntaxKind {
  Uknown,

  ExprConstNumber,
  ExprBinary,
};

class SyntaxNode {
 private:
  const SyntaxKind kind_;

 protected:
  explicit SyntaxNode(const SyntaxKind kind) : kind_{kind} {}
};

class ExpressionNode : public SyntaxNode {
 protected:
  explicit ExpressionNode(const SyntaxKind kind) : SyntaxNode{kind} {}
};

class ConstantExpressionNode : public ExpressionNode {
 public:
  using ValueType = std::shared_ptr<const std::string>;

 private:
  ValueType value_;

 protected:
  explicit ConstantExpressionNode(const SyntaxKind kind, const ValueType& value)
      : ExpressionNode{kind}, value_{value} {}
};

class ConstantNumberExpressionNode : public ConstantExpressionNode {
 public:
  explicit ConstantNumberExpressionNode(const ValueType& value)
      : ConstantExpressionNode{SyntaxKind::ExprConstNumber, value} {}
};

#pragma region Parser Context

using LexicalTokenPredicate = Predicate<const LexicalToken&>;

class ParserContext;
using ParserState = State<ParserContext>;

class ParserContext final
    : public EnumeratingContext<ParserContext, LexicalToken> {
 public:
  using StateStack = std::stack<ParserState>;
  using ExpressionStack = std::stack<std::shared_ptr<ExpressionNode>>;

 private:
  const TokenCollection tokens_;
  bool started_;
  std::vector<LexicalToken>::const_iterator current_;

 public:
  StateStack state_stack;
  ExpressionStack expression_stack;

  explicit ParserContext(const TokenCollection& tokens);
  virtual ~ParserContext() = default;

  auto current() -> const LexicalToken& override;
  auto move_next() -> bool override;
};

#pragma endregion

class Parser final : public StateMachine<ParserContext> {
 public:
  Parser();
};

#else
#error "!cplusplus"
#endif