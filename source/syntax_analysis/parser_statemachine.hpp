// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "../lexical_analysis/token.hpp"
#include "../state_machine.hpp"

#include "syntax.hpp"

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
  //using ExpressionStack = std::stack<std::shared_ptr<ExpressionNode>>;
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
  //ExpressionStack expression_stack;

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

  template <IsSyntaxNode T>
  auto get_syntax_node() const -> T* {
    auto* ptr = dynamic_cast<T*>(syntax_stack.top().get());
    assert(ptr);
    return ptr;
  }

  template <IsSyntaxNode T>
  auto pop_syntax_node() -> std::shared_ptr<T> {
    auto node = std::move(syntax_stack.top());
    syntax_stack.pop();
    auto tnode = std::dynamic_pointer_cast<T>(node);
    assert(tnode);
    return tnode;
  }

  auto get_var_def_node() { return get_syntax_node<VarDefinition>(); }

  auto get_func_def_node() { return get_syntax_node<FuncDefinition>(); }
  auto get_func_param_node() { return get_syntax_node<FuncParameter>(); }
  auto pop_func_def_param() { return pop_syntax_node<FuncParameter>(); }

  auto get_statement_node() { return get_syntax_node<Statement>(); }
  auto pop_statement_node() { return pop_syntax_node<Statement>(); }

  auto get_statement_return_node() {
    return get_syntax_node<ReturnStatement>();
  }

  auto pop_expr_node() { return pop_syntax_node<ExpressionNode>(); }
  auto pop_expr_binary_node() { return pop_syntax_node<BinaryExpression>(); }
};

#pragma endregion

class Parser final : public StateMachine<ParserContext> {
 public:
  Parser();
};

using LexicalTokenPredicate = Predicate<const LexicalToken&>;

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

constexpr LexicalTokenPredicate is_paren_open = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolParenOpen);
};

constexpr LexicalTokenPredicate is_paren_close = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolParenClose);
};

constexpr LexicalTokenPredicate is_brace_open = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolBraceOpen);
};

constexpr LexicalTokenPredicate is_brace_close = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolBraceClose);
};

constexpr LexicalTokenPredicate is_bracket_open = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolBracketOpen);
};

constexpr LexicalTokenPredicate is_bracket_close = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolBracketClose);
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

constexpr LexicalTokenPredicate is_keyword_return = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordReturn);
};

// todo : reorder kind enum to optimize jump table
constexpr LexicalTokenPredicate is_binary_operator = [](auto& token) {
  auto kind = token.kind();
  switch (kind) {
    case LexicalKind::SymbolPlus:
    case LexicalKind::SymbolMinus:
    case LexicalKind::SymbolStar:
    case LexicalKind::SymbolSlash:

    case LexicalKind::SymbolBoolEquals:
    case LexicalKind::SymbolBoolNotEquals:
    case LexicalKind::SymbolBoolOr:
    case LexicalKind::SymbolBoolAnd:

    case LexicalKind::SymbolAngleOpen:
    case LexicalKind::SymbolAngleClose:
    case LexicalKind::SymbolLessThanEqual:
    case LexicalKind::SymbolGreaterThanEqual:

    case LexicalKind::SymbolBitOr:
    case LexicalKind::SymbolBitAnd:
    case LexicalKind::SymbolBitXor:

    case LexicalKind::SymbolShiftLeft:
    case LexicalKind::SymbolShiftRight: {
      return true;
    }

    default:
      return false;
  }
};
