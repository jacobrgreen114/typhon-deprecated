// Copyright (c) 2023 Jacob R. Green
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

class ParserContext final : public EnumeratingContext<ParserContext, LexicalToken> {
 public:
  using Source          = std::shared_ptr<SourceNode>;
  using StateStack      = std::stack<ReturnState>;
  using SyntaxStack     = std::stack<std::shared_ptr<SyntaxNode>>;
  using PrecedenceStack = std::stack<Precedence>;

 private:
  const TokenCollection tokens_;
  bool started_;
  std::vector<LexicalToken>::const_iterator current_;

  StateStack state_stack;

 public:
  // todo : privitize fields

  Source source;

  SyntaxStack syntax_stack;
  PrecedenceStack precedence_stack;

  explicit ParserContext(const TokenCollection& tokens);
  virtual ~ParserContext() = default;

  auto current() -> const LexicalToken& override;
  auto move_next() -> bool override;

  auto push_states(ParserState ret_state, ParserState end_state) {
    state_stack.push({ret_state, end_state});
  }

 private:
  auto pop_states() {
    auto state = state_stack.top();
    state_stack.pop();
    return state;
  }

 public:
  auto pop_ret_state() -> ParserState { return pop_states().ret; }

  auto pop_end_state() -> ParserState { return pop_states().end; }

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
  auto pop_def_node() { return pop_syntax_node<Definition>(); }

  auto get_func_def_node() { return get_syntax_node<FuncDefinition>(); }
  auto get_func_param_node() { return get_syntax_node<FuncParameter>(); }
  auto pop_func_def_param() { return pop_syntax_node<FuncParameter>(); }

  auto get_statement_node() { return get_syntax_node<Statement>(); }
  auto pop_statement_node() { return pop_syntax_node<Statement>(); }

  auto get_statement_def_node() { return get_syntax_node<DefStatement>(); }
  auto get_statement_expr_node() { return get_syntax_node<ExprStatement>(); }

  auto get_statement_return_node() { return get_syntax_node<ReturnStatement>(); }

  auto get_statement_block() { return get_syntax_node<StatementBlock>(); }
  auto pop_statement_block() { return pop_syntax_node<StatementBlock>(); }

  auto get_statement_if_node() { return get_syntax_node<IfStatement>(); }
  auto get_statement_elif_node() { return get_syntax_node<ElifStatement>(); }
  auto get_statement_else_node() { return get_syntax_node<ElseStatement>(); }

  auto pop_expr_node() { return pop_syntax_node<ExpressionNode>(); }

  auto get_expr_unary_node() { return get_syntax_node<UnaryExpression>(); }

  auto pop_expr_binary_node() { return pop_syntax_node<BinaryExpression>(); }
};

#pragma endregion

class Parser final : public StateMachine<ParserContext> {
 public:
  Parser();
};

using LexicalTokenPredicate = Predicate<const LexicalToken&>;

constexpr auto is_token_kind(const LexicalToken& token, const LexicalKind kind) -> bool {
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

constexpr LexicalTokenPredicate is_keyword_if = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordIf);
};
constexpr LexicalTokenPredicate is_keyword_elif = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordElif);
};

constexpr LexicalTokenPredicate is_keyword_else = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordElse);
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

auto is_unary_pre_operator(const LexicalToken& token) -> bool;
auto is_unary_post_operator(const LexicalToken& token) -> bool;

auto is_binary_operator(const LexicalToken& token) -> bool;