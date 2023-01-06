// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "syntax_analysis.hpp"

#include <array>

#pragma region Predicates

constexpr auto is_token_kind(const LexicalToken& token, const LexicalKind kind)
    -> bool {
  return token.kind() == kind;
};

constexpr LexicalTokenPredicate is_identifier = [](auto& token) {
  return is_token_kind(token, LexicalKind::Identifier);
};
constexpr LexicalTokenPredicate is_colon = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolColon);
};
constexpr LexicalTokenPredicate is_equals = [](auto& token) {
  return is_token_kind(token, LexicalKind::SymbolEquals);
};

constexpr LexicalTokenPredicate is_keyword_var = [](auto& token) {
  return is_token_kind(token, LexicalKind::KeywordVar);
};

#pragma endregion

#pragma region Global Parser States

constexpr auto error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

constexpr auto unexpected_token_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

constexpr auto exit_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

extern const ParserState definition_var_start_state;

constexpr auto unknown_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto kind = ctx.current().kind();

      switch (kind) {
        case LexicalKind::KeywordVar:
          return definition_var_start_state;
        case LexicalKind::KeywordFunc:
        case LexicalKind::KeywordObject:
          throw_not_implemented();
        default: {
          return unexpected_token_error_state;
        }
      }
    }};

constexpr auto start_state = ParserState{[](ParserContext& ctx) -> ParserState {
  return ctx.move_next_state(unknown_state, exit_state);
}};

#pragma endregion

#pragma region Parser Variable Definition States

constexpr ParserState definition_var_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

constexpr ParserState definition_var_unexpected_end_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

constexpr ParserState definition_var_assign_start_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

constexpr ParserState definition_var_type_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_identifier(ctx.current()));
      auto type_name = ctx.current().value();
      throw_not_implemented();
    }};

constexpr ParserState definition_var_type_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_colon(ctx.current()));
      return ctx.move_next_state(is_identifier, definition_var_type_state,
                                 definition_var_error_state,
                                 definition_var_unexpected_end_error_state);
    }};

constexpr ParserState definition_var_identifier_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_identifier(ctx.current()));
      auto identifier_name = ctx.current().value();

      constexpr auto match_conditions =
          std::array<ParserContext::RefMatchCondition, 2>{
              ParserContext::RefMatchCondition{is_colon,
                                               definition_var_type_start_state},
              ParserContext::RefMatchCondition{
                  is_equals, definition_var_assign_start_state}};

      return ctx.move_next_state(definition_var_error_state,
                                 definition_var_unexpected_end_error_state,
                                 match_conditions);
    }};

constexpr ParserState definition_var_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_keyword_var(ctx.current()));
      return ctx.move_next_state(is_identifier, definition_var_identifier_state,
                                 definition_var_error_state,
                                 definition_var_unexpected_end_error_state);
    }};

#pragma endregion

#pragma region Parser Context

ParserContext::ParserContext(const std::vector<LexicalToken>& tokens)
    : tokens_{tokens}, started_{false}, current_{} {}

auto ParserContext::current() -> const LexicalToken& { return *current_; }

auto ParserContext::move_next() -> bool {
  if (!started_) {
    current_ = tokens_.begin();
    started_ = true;
    return true;
  }
  return ++current_ < tokens_.end();
}

#pragma endregion

#pragma region Parser

Parser::Parser() : StateMachine<ParserContext>(start_state) {}

#pragma endregion