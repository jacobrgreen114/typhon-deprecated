// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "parser_statement.hpp"

#include "parser_expression.hpp"
#include "parser_def_func.hpp"

auto statement_unexpected_end_error_handler_(ParserContext& ctx)
    -> ParserState {
  throw_not_implemented();
}

static constexpr ParserState statement_unexpected_end_error_state =
    ParserState{statement_unexpected_end_error_handler_};

auto statement_expected_semicolon_error_handler_(ParserContext& ctx)
    -> ParserState {
  // todo : redo error message
  const auto& current = ctx.current();
  std::cerr << "Error : Expected semicolon " << current.pos() << std::endl;
  assert(false);
  exit(-1);
}

static constexpr ParserState statement_expected_semicolon_error_state =
    ParserState{statement_expected_semicolon_error_handler_};

/*
 * Return Statement
 */

auto statement_return_end_handler_(ParserContext& ctx) -> ParserState {
  if (!is_semicolon(ctx.current())) {
    return statement_expected_semicolon_error_state;
  }

  auto expr = ctx.pop_expr_node();
  auto ret = ctx.get_statement_return_node();
  ret->set_expr(expr);
  return ctx.move_next_stack();
}

static constexpr ParserState statement_return_end_state =
    ParserState{statement_return_end_handler_};

auto statement_return_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_return(ctx.current()));
  ctx.syntax_stack.emplace(std::make_shared<ReturnStatement>());
  ctx.push_states(statement_return_end_state,
                  statement_unexpected_end_error_state);
  return ctx.move_next_state(expr_start_state,
                             statement_unexpected_end_error_state);
}

static constexpr ParserState statement_return_state =
    ParserState{statement_return_handler_};

/*
 * Expression Statement
 */

auto statement_expr_end_handler_(ParserContext& ctx) -> ParserState {
  if (!is_semicolon(ctx.current())) {
    return statement_expected_semicolon_error_state;
  }

  auto expr = ctx.pop_expr_node();
  auto ret = ctx.get_statement_expr_node();
  ret->set_expr(expr);
  return ctx.move_next_stack();
}

static constexpr ParserState statement_expr_end_state =
    ParserState{statement_expr_end_handler_};

auto statement_expr_handler_(ParserContext& ctx) -> ParserState {
  ctx.syntax_stack.push(std::make_shared<ExprStatement>());
  ctx.push_states(statement_expr_end_state,
                  statement_unexpected_end_error_state);
  return expr_start_state;
}

static constexpr ParserState statement_expr_state =
    ParserState{statement_expr_handler_};

/*
 * Statement Start
 */

auto statement_unknown_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  auto kind = current.kind();

  // ctx.push_states(statement_end_state, statement_end_exit_state);
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
      return statement_expr_state;
    }
  }

  throw_not_implemented();
}

static constexpr ParserState statement_unknown_state =
    ParserState{statement_unknown_handler_};

static constexpr ParserState statement_start_state =
    ParserState{statement_unknown_handler_};

/*
 * Statement Block
 */

auto statement_block_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

constexpr ParserState statement_block_error_state =
    ParserState{statement_block_error_handler_};

auto statement_block_unexpected_end_error_handler_(ParserContext& ctx)
    -> ParserState {
  throw_not_implemented();
}

constexpr ParserState statement_block_unexpected_end_error_state =
    ParserState{statement_block_unexpected_end_error_handler_};

auto statement_block_end_handler_(ParserContext& ctx) -> ParserState {
  return ctx.move_next_stack();
}

constexpr ParserState statement_block_end_state =
    ParserState{statement_block_end_handler_};

extern const ParserState statement_block_possible_end_state;

auto statement_block_statement_end_handler_(ParserContext& ctx) -> ParserState {
  auto statement = ctx.pop_statement_node();
  auto* block = ctx.get_statement_block();
  block->push_statement(statement);
  return statement_block_possible_end_state;
}

constexpr ParserState statement_block_statement_end_state =
    ParserState{statement_block_statement_end_handler_};

auto statement_block_statement_handler_(ParserContext& ctx) -> ParserState {
  ctx.push_states(statement_block_statement_end_state,
                  statement_block_unexpected_end_error_state);
  return statement_start_state;
}

constexpr ParserState statement_block_statement_state =
    ParserState{statement_block_statement_handler_};

auto statement_block_possible_end_handler_(ParserContext& ctx) -> ParserState {
  if (is_bracket_close(ctx.current())) {
    return statement_block_end_state;
  } else {
    return statement_block_statement_state;
  }

  // return ctx.move_next_state(is_bracket_close, statement_block_end_state,
  //                            statement_block_statement_state,
  //                            statement_block_unexpected_end_error_state);
}

constexpr ParserState statement_block_possible_end_state =
    ParserState{statement_block_possible_end_handler_};

auto statement_block_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_bracket_open(ctx.current()));

  ctx.syntax_stack.push(std::make_shared<StatementBlock>());
  return ctx.move_next_state(statement_block_possible_end_state,
                             statement_block_unexpected_end_error_state);
  //return statement_block_possible_end_state;
}

constexpr ParserState statement_block_start_state =
    ParserState{statement_block_start_handler_};