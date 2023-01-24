// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include <stack>

#include "lexical_analysis.hpp"
#include "state_machine.hpp"

enum class SyntaxKind {
  Unknown,

  Source,

  ExprConstNumber,
  ExprBinary,

  Statement,
  StatementBlock,

  DefVar,
  DefFunc,
  DefParam,
  DefStruct,
  DefClass,
  DefInterface,
};

class SyntaxNode {
 private:
  const SyntaxKind kind_;

 protected:
  explicit constexpr SyntaxNode(const SyntaxKind kind) : kind_{kind} {}

 public:
  virtual ~SyntaxNode() = default;

  constexpr auto kind() const noexcept { return kind_; }
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

class Definition : public SyntaxNode {
  String name_;

 protected:
  Definition(SyntaxKind kind, const String& name = nullptr)
      : SyntaxNode(kind), name_{name} {}

 public:
  auto set_name(const String& name) noexcept -> void { name_ = name; }
};

class VarDefinition : public Definition {
 public:
  using Assignment = std::shared_ptr<ExpressionNode>;

 private:
  String type_name_;
  Assignment assignment_;

 public:
  explicit VarDefinition(const String& name = nullptr,
                         const String& type_name = nullptr,
                         const Assignment& assign = nullptr)
      : Definition(SyntaxKind::DefVar, name),
        type_name_{type_name},
        assignment_{assign} {}

 public:
  auto set_type_name(const String& type_name) noexcept -> void {
    type_name_ = type_name;
  }

  auto set_assignment(const Assignment& assignment) noexcept -> void {
    assignment_ = assignment;
  }
};

class FuncParameter final : public Definition {
 private:
  String type_name_;

 public:
  explicit FuncParameter(const String& name = nullptr)
      : Definition(SyntaxKind::DefParam, name) {}

  auto set_type_name(const String& type_name) noexcept -> void {
    type_name_ = type_name;
  }
};

class Statement : public SyntaxNode {
 public:
  explicit Statement() : SyntaxNode(SyntaxKind::Statement) {}
};

class ReturnStatement final : public Statement {

};

class VarStatement final : public Statement {

};

class BodyStatement : public Statement {

};

class IfStatement final : public BodyStatement {

};

class ElifStatement final : public BodyStatement {

};

class ElseStatement final : public BodyStatement {

};

class LoopStatement final : public BodyStatement {

};

class WhileStatement final : public BodyStatement {

};

class ForStatement final : public BodyStatement {

};

class ForeachStatement final : public BodyStatement {

};

class StatementBlock final : public SyntaxNode {
 public:
  using Statement = std::shared_ptr<Statement>;
  using StatementCollection = std::vector<Statement>;

 private:
  StatementCollection statements_;

 public:
  explicit StatementBlock() : SyntaxNode(SyntaxKind::StatementBlock) {}
  auto push_statement(const Statement& statement) -> void {
    statements_.emplace_back(statement);
  }
};

class FuncDefinition final : public Definition {
 public:
  using Parameter = std::shared_ptr<FuncParameter>;
  using ParameterCollection = std::vector<Parameter>;

 private:
  ParameterCollection parameters_;
  String return_;

 public:
  explicit FuncDefinition() : Definition(SyntaxKind::DefFunc) {}

  auto set_return_type(const String& ret_type) -> void { return_ = ret_type; }

  auto push_parameter(const Parameter& param) -> void {
    parameters_.emplace_back(param);
  }
};

class SourceNode final : public SyntaxNode {
  using Node = std::shared_ptr<SyntaxNode>;
  using NodeArray = std::vector<Node>;

  NodeArray nodes_;

 public:
  constexpr SourceNode() : SyntaxNode(SyntaxKind::Source) {}

  auto push_node(const Node& node) { nodes_.emplace_back(node); }
};

#pragma region Parser Context

using LexicalTokenPredicate = Predicate<const LexicalToken&>;

class ParserContext;
using ParserState = State<ParserContext>;

struct ReturnState {
  ParserState ret;
  ParserState end;
};

class ParserContext final
    : public EnumeratingContext<ParserContext, LexicalToken> {
 public:
  using Source = std::shared_ptr<SourceNode>;
  using StateStack = std::stack<ReturnState>;
  using ExpressionStack = std::stack<std::shared_ptr<ExpressionNode>>;
  using SyntaxStack = std::stack<std::shared_ptr<SyntaxNode>>;

 private:
  const TokenCollection tokens_;
  bool started_;
  std::vector<LexicalToken>::const_iterator current_;

  StateStack state_stack;

 public:
  // todo : privitize fields

  Source source;

  SyntaxStack syntax_stack;
  ExpressionStack expression_stack;

  explicit ParserContext(const TokenCollection& tokens);
  virtual ~ParserContext() = default;

  auto current() -> const LexicalToken& override;
  auto move_next() -> bool override;

  auto push_states(ParserState ret_state, ParserState end_state) {
    state_stack.push({ret_state, end_state});
  }

  auto pop_states() {
    auto state = state_stack.top();
    state_stack.pop();
    return state;
  }

  auto move_next_stack() -> ParserState {
    auto states = pop_states();
    return move_next_state(states.ret, states.end);
  }
};

#pragma endregion

class Parser final : public StateMachine<ParserContext> {
 public:
  Parser();
};

constexpr auto is_token_kind(const LexicalToken& token, const LexicalKind kind)
    -> bool {
  return token.kind() == kind;
};

constexpr LexicalTokenPredicate is_identifier = [](auto& token) {
  return is_token_kind(token, LexicalKind::Identifier);
};

constexpr LexicalTokenPredicate is_semicolon = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolSemicolon);
};

constexpr LexicalTokenPredicate is_colon = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolColon);
};

constexpr LexicalTokenPredicate is_comma = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolComma);
};

constexpr LexicalTokenPredicate is_brace_open = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolOpenBrace);
};

constexpr LexicalTokenPredicate is_brace_close = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolCloseBrace);
};

constexpr LexicalTokenPredicate is_paren_open = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolOpenParen);
};

constexpr LexicalTokenPredicate is_paren_close = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolCloseParen);
};

constexpr LexicalTokenPredicate is_arrow = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolArrow);
};

constexpr LexicalTokenPredicate is_equals = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolEquals);
};

constexpr LexicalTokenPredicate is_keyword_var = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordVar);
};

constexpr LexicalTokenPredicate is_keyword_func = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordFunc);
};

#define is ==

constexpr LexicalTokenPredicate is_binary_operator = [](auto& token) {
  auto kind = token.kind();
  return kind is LexicalKind::SymbolAdd or
         kind is LexicalKind::SymbolSubtract or
         kind is LexicalKind::SymbolMultiply or
         kind is LexicalKind::SymbolDivide;
};