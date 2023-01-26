// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "parser_statement.hpp"

#include "parser_expression.hpp"
#include "parser_def_func.hpp"

static constexpr ParserState statement_block_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_block_unexpected_end_error_state =
    ParserState{
        [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_block_end_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_unexpected_end_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState statement_end_exit_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

auto statement_end_handler_(ParserContext& ctx) -> ParserState {
  assert(is_semicolon(ctx.current()));
  return ctx.move_next_stack();
}

static constexpr ParserState statement_end_state =
    ParserState{statement_end_handler_};

auto statement_return_end_handler_(ParserContext& ctx) -> ParserState {
  auto expr = ctx.pop_expr_node();
  auto ret = ctx.get_statement_return_node();
  ret->set_expr(expr);
  return statement_end_state;
}

static constexpr ParserState statement_return_end_state =
    ParserState{statement_return_end_handler_};

auto statement_return_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_return(ctx.current()));
  ctx.syntax_stack.emplace(std::make_shared<ReturnStatement>());
  ctx.push_states(statement_return_end_state,
                  statement_block_unexpected_end_error_state);
  return ctx.move_next_state(expr_start_state,
                             statement_unexpected_end_error_state);
}

static constexpr ParserState statement_return_state =
    ParserState{statement_return_handler_};

auto statement_unknown_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  auto kind = current.kind();

  ctx.push_states(statement_end_state, statement_end_exit_state);
  switch (kind) {
    case LexicalKind::KeywordVar: {
      throw_not_implemented();
    }
    case LexicalKind::KeywordReturn: {
      return statement_return_state;
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
      return expr_start_state;
    }
  }

  throw_not_implemented();
}

static constexpr ParserState statement_unknown_state =
    ParserState{statement_unknown_handler_};

auto statement_block_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_bracket_open(current));

  ctx.syntax_stack.push(std::make_shared<StatementBlock>());
  return ctx.move_next_state(is_brace_close, statement_block_end_state,
                             statement_unknown_state,
                             statement_block_unexpected_end_error_state);
}

static constexpr ParserState statement_block_start_state =
    ParserState{statement_block_start_handler_};