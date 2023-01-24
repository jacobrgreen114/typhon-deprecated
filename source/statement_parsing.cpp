// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "statement_parsing.hpp"

#include "expression_parsing.hpp"
#include "fndef_parsing.hpp"

static constexpr ParserState statement_block_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_block_unexpected_end_error_state =
    ParserState{
        [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_block_end_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_end_exit_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_end_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_unknown_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      auto kind = current.kind();

      ctx.push_states(statement_end_state, statement_end_exit_state);
      switch (kind) {
        case LexicalKind::KeywordVar: {
          return func_def_start_state;
        }
        case LexicalKind::KeywordReturn: {
          throw_not_implemented();
        }
        case LexicalKind::KeywordIf: {
          throw_not_implemented();
        }
        case LexicalKind::KeywordElif: {
          throw_not_implemented();
        }
        case LexicalKind::KeywordElse: {
          throw_not_implemented();
        }
        case LexicalKind::KeywordLoop: {
          throw_not_implemented();
        }
        case LexicalKind::KeywordWhile: {
          throw_not_implemented();
        }
        case LexicalKind::KeywordFor: {
          throw_not_implemented();
        }
        case LexicalKind::KeywordForeach: {
          throw_not_implemented();
        }
        case LexicalKind::SymbolSemicolon: {
          throw_not_implemented();
        }
        default: {
          return expression_start_state;
        }
      }

      throw_not_implemented();
    }};

static constexpr ParserState statement_block_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_brace_open(current));

      ctx.syntax_stack.push(std::make_shared<StatementBlock>());
      return ctx.move_next_state(is_brace_close, statement_block_end_state,
                                 statement_unknown_state,
                                 statement_block_unexpected_end_error_state);
    }};