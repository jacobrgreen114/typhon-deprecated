// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "token.hpp"
#include "state_machine.hpp"

#include "syntax_tree.hpp"

class ParserContext;
using ParserState = State<ParserContext>;

struct ReturnState {
  ParserState ret;
  ParserState end;
};

class ParserContext final : public EnumeratingContext<ParserContext, LexicalToken> {
 public:
  using Source          = std::unique_ptr<SyntaxTree>;
  using StateStack      = std::stack<ReturnState>;
  using SyntaxStack     = std::stack<std::unique_ptr<BaseSyntax>>;
  using PrecedenceStack = std::stack<Precedence>;
  using TokenStack      = std::stack<LexicalToken>;

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
  TokenStack token_stack;

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
  auto push_current_token() -> void { token_stack.emplace(current()); }

  auto pop_token_stack() -> LexicalToken {
    auto token = token_stack.top();
    token_stack.pop();
    return token;
  }

  auto pop_ret_state() -> ParserState { return pop_states().ret; }

  auto pop_end_state() -> ParserState { return pop_states().end; }

  auto move_next_stack() -> ParserState {
    auto states = pop_states();
    return move_next_state(states.ret, states.end);
  }

  template <IsSyntaxNode T>
  auto get_syntax_node() const -> T& {
    return deref(ptr_cast<T>(syntax_stack.top().get()));
  }

  template <IsSyntaxNode T>
  auto pop_syntax_node() -> std::unique_ptr<T> {
    auto tnode = ptr_cast<T>(std::move(syntax_stack.top()));
    syntax_stack.pop();
    return tnode;
  }
};

using ParserMatchCondition = ParserContext::RefMatchCondition;

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

constexpr LexicalTokenPredicate is_doublecolon = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolDoubleColon);
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

constexpr LexicalTokenPredicate is_square_open = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolSquareOpen);
};

constexpr LexicalTokenPredicate is_square_close = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolsquareClose);
};

constexpr LexicalTokenPredicate is_curly_open = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolCurlyOpen);
};

constexpr LexicalTokenPredicate is_curly_close = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolCurlyClose);
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

constexpr LexicalTokenPredicate is_keyword_loop = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordLoop);
};

constexpr LexicalTokenPredicate is_keyword_while = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordWhile);
};

constexpr LexicalTokenPredicate is_keyword_for = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordFor);
};

constexpr LexicalTokenPredicate is_keyword_foreach = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordForeach);
};

constexpr LexicalTokenPredicate is_keyword_var = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordVar);
};

constexpr LexicalTokenPredicate is_keyword_func = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordFunc);
};

constexpr LexicalTokenPredicate is_keyword_struct = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordStruct);
};

constexpr LexicalTokenPredicate is_keyword_object = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordObject);
};

constexpr LexicalTokenPredicate is_keyword_return = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordReturn);
};

constexpr LexicalTokenPredicate is_keyword_mut = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordMutable);
};

constexpr LexicalTokenPredicate is_keyword_cinclude = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordCInclude);
};

constexpr LexicalTokenPredicate is_keyword_ctype = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordCType);
};

constexpr LexicalTokenPredicate is_number = [](auto& token) {
  return is_token_kind(token, LexicalKind::Number);
};

constexpr LexicalTokenPredicate is_string = [](auto& token) {
  return is_token_kind(token, LexicalKind::String);
};

auto is_unary_pre_operator(const LexicalToken& token) -> bool;
auto is_unary_post_operator(const LexicalToken& token) -> bool;

auto is_binary_operator(const LexicalToken& token) -> bool;

auto is_def_modifier(const LexicalToken& token) -> bool;

extern const ParserState error_state;
extern const ParserState unexpected_token_error_state;