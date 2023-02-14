// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_stmt.hpp"

#include "parser_expression.hpp"
#include "parser_def_func.hpp"
#include "parser_def_var.hpp"

extern const ParserState statement_start_state;

/*
 * Exit
 */

auto statement_error_handler_(ParserContext& ctx) -> ParserState;
auto statement_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState;
auto statement_expected_semicolon_error_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_error_state = ParserState{statement_error_handler_};

constexpr auto statement_unexpected_end_error_state =
    ParserState{statement_unexpected_end_error_handler_};

constexpr auto statement_expected_semicolon_error_state =
    ParserState{statement_expected_semicolon_error_handler_};

auto statement_error_handler_(ParserContext& ctx) -> ParserState { return error_state; }

auto statement_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

auto statement_expected_semicolon_error_handler_(ParserContext& ctx) -> ParserState {
  // todo : redo error message
  const auto& current = ctx.current();
  std::cerr << "Error : Expected semicolon " << current.pos() << std::endl;
  assert(false);
  exit(-1);
}

/*
 * Body Statement
 */

auto append_body_to_statement(ParserContext& ctx) {
  auto block = ctx.pop_syntax_node<StatementBlock>();
  ctx.get_syntax_node<BaseBodyStatement>().set_body(std::move(block));
}

/*
 * If Statement
 */

auto statement_if_error_handler_(ParserContext& ctx) -> ParserState;
auto statement_if_body_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_if_body_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_if_expr_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_if_expr_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_if_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_if_error_state      = ParserState{statement_if_error_handler_};
constexpr auto statement_if_body_end_state   = ParserState{statement_if_body_end_handler_};
constexpr auto statement_if_body_start_state = ParserState{statement_if_body_start_handler_};
constexpr auto statement_if_expr_end_state   = ParserState{statement_if_expr_end_handler_};
constexpr auto statement_if_expr_start_state = ParserState{statement_if_expr_start_handler_};
constexpr auto statement_if_state            = ParserState{statement_if_handler_};

auto statement_if_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto statement_if_body_end_handler_(ParserContext& ctx) -> ParserState {
  append_body_to_statement(ctx);
  return ctx.pop_ret_state();
}

auto statement_if_body_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_open(ctx.current()));
  ctx.push_states(statement_if_body_end_state, statement_unexpected_end_error_state);
  return statement_block_start_state;
}

auto statement_if_expr_end_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_close(ctx.current()));
  auto expr = ctx.pop_syntax_node<BaseExpression>();
  ctx.get_syntax_node<IfStatement>().set_expr(std::move(expr));

  return ctx.move_next_state(is_curly_open,
                             statement_if_body_start_state,
                             statement_if_error_state,
                             statement_unexpected_end_error_state);
}

auto statement_if_expr_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_open(ctx.current()));
  ctx.push_states(statement_if_expr_end_state, statement_unexpected_end_error_state);
  return ctx.move_next_state(expr_start_state, statement_unexpected_end_error_state);
}

auto statement_if_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_if(ctx.current()));
  ctx.syntax_stack.emplace(std::make_unique<IfStatement>(ctx.current().pos()));
  return ctx.move_next_state(is_paren_open,
                             statement_if_expr_start_state,
                             statement_if_error_state,
                             statement_unexpected_end_error_state);
}

/*
 * Elif Statement
 */

auto statement_elif_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_elif_state = ParserState{statement_elif_handler_};

auto statement_elif_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_elif(ctx.current()));
  ctx.syntax_stack.emplace(std::make_unique<ElifStatement>(ctx.current().pos()));
  return ctx.move_next_state(is_paren_open,
                             statement_if_expr_start_state,
                             statement_if_error_state,
                             statement_unexpected_end_error_state);
}

/*
 * Else Statement
 */

auto statement_else_body_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_else_body_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_else_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_else_body_end_state   = ParserState{statement_else_body_end_handler_};
constexpr auto statement_else_body_start_state = ParserState{statement_else_body_start_handler_};
constexpr auto statement_else_state            = ParserState{statement_else_handler_};

auto statement_else_body_end_handler_(ParserContext& ctx) -> ParserState {
  append_body_to_statement(ctx);
  return ctx.pop_ret_state();
}

auto statement_else_body_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_open(ctx.current()));
  ctx.push_states(statement_else_body_end_state, statement_unexpected_end_error_state);
  return statement_block_start_state;
}

auto statement_else_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_else(ctx.current()));
  ctx.syntax_stack.emplace(std::make_unique<ElseStatement>(ctx.current().pos()));
  return ctx.move_next_state(is_curly_open,
                             statement_else_body_start_state,
                             statement_error_state,
                             statement_unexpected_end_error_state);
}

/*
 * Return Statement
 */

auto statement_return_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_return_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_return_end_state = ParserState{statement_return_end_handler_};
constexpr auto statement_return_state     = ParserState{statement_return_handler_};

auto statement_return_end_handler_(ParserContext& ctx) -> ParserState {
  if (!is_semicolon(ctx.current())) {
    return statement_expected_semicolon_error_state;
  }

  auto expr = ctx.pop_syntax_node<BaseExpression>();
  ctx.get_syntax_node<ReturnStatement>().set_expr(std::move(expr));

  return ctx.move_next_stack();
}

auto statement_return_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_return(ctx.current()));
  ctx.syntax_stack.emplace(std::make_unique<ReturnStatement>(ctx.current().pos()));
  ctx.push_states(statement_return_end_state, statement_unexpected_end_error_state);
  return ctx.move_next_state(expr_start_state, statement_unexpected_end_error_state);
}

/*
 * Expression Statement
 */

auto statement_expr_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_expr_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_expr_end_state = ParserState{statement_expr_end_handler_};
constexpr auto statement_expr_state     = ParserState{statement_expr_handler_};

auto statement_expr_end_handler_(ParserContext& ctx) -> ParserState {
  if (!is_semicolon(ctx.current())) {
    return statement_expected_semicolon_error_state;
  }

  auto expr = ctx.pop_syntax_node<BaseExpression>();
  ctx.get_syntax_node<ExpressionStatement>().set_expr(std::move(expr));

  return ctx.move_next_stack();
}

auto statement_expr_handler_(ParserContext& ctx) -> ParserState {
  ctx.syntax_stack.push(std::make_unique<ExpressionStatement>(ctx.current().pos()));
  ctx.push_states(statement_expr_end_state, statement_unexpected_end_error_state);
  return expr_start_state;
}

/*
 * Definition Statement
 */

auto append_definition(ParserContext& ctx) -> void {
  auto def = ctx.pop_syntax_node<BaseDefinition>();
  ctx.get_syntax_node<DefinitionStatement>().set_def(std::move(def));
}

/*
 * Var Definition Statement
 */

auto statement_def_var_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_def_var_start_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_def_var_end_state = ParserState{statement_def_var_end_handler_};
constexpr auto statement_def_var_state     = ParserState{statement_def_var_start_handler_};

auto statement_def_var_end_handler_(ParserContext& ctx) -> ParserState {
  append_definition(ctx);
  return ctx.pop_ret_state();
}

auto statement_def_var_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_keyword_var(current));
  ctx.syntax_stack.emplace(std::make_unique<DefinitionStatement>(ctx.current().pos()));
  ctx.push_states(statement_def_var_end_state, statement_unexpected_end_error_state);
  return var_def_start_state;
}

/*
 * Loop Statement
 */

auto statement_loop_body_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_loop_body_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_loop_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_loop_body_end_state   = ParserState{statement_loop_body_end_handler_};
constexpr auto statement_loop_body_start_state = ParserState{statement_loop_body_start_handler_};
constexpr auto statement_loop_state            = ParserState{statement_loop_handler_};

auto statement_loop_body_end_handler_(ParserContext& ctx) -> ParserState {
  append_body_to_statement(ctx);
  return ctx.pop_ret_state();
}

auto statement_loop_body_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_open(ctx.current()));
  ctx.push_states(statement_loop_body_end_state, statement_unexpected_end_error_state);
  return statement_block_start_state;
}

auto statement_loop_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_loop(ctx.current()));
  ctx.syntax_stack.emplace(std::make_unique<LoopStatement>(ctx.current().pos()));
  return ctx.move_next_state(is_curly_open,
                             statement_loop_body_start_state,
                             statement_error_state,
                             statement_unexpected_end_error_state);
}

/*
 * While Statement
 */

auto statement_while_error_handler_(ParserContext& ctx) -> ParserState;
auto statement_while_body_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_while_body_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_while_expr_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_while_expr_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_while_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_while_error_state      = ParserState{statement_while_error_handler_};
constexpr auto statement_while_body_end_state   = ParserState{statement_while_body_end_handler_};
constexpr auto statement_while_body_start_state = ParserState{statement_while_body_start_handler_};
constexpr auto statement_while_expr_end_state   = ParserState{statement_while_expr_end_handler_};
constexpr auto statement_while_expr_start_state = ParserState{statement_while_expr_start_handler_};
constexpr auto statement_while_state            = ParserState{statement_while_handler_};

auto statement_while_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto statement_while_body_end_handler_(ParserContext& ctx) -> ParserState {
  append_body_to_statement(ctx);
  return ctx.pop_ret_state();
}

auto statement_while_body_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_open(ctx.current()));
  ctx.push_states(statement_while_body_end_state, statement_unexpected_end_error_state);
  return statement_block_start_state;
}

auto statement_while_expr_end_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_close(ctx.current()));
  auto expr = ctx.pop_syntax_node<BaseExpression>();
  ctx.get_syntax_node<WhileStatement>().set_expr(std::move(expr));

  return ctx.move_next_state(is_curly_open,
                             statement_while_body_start_state,
                             statement_while_error_state,
                             statement_unexpected_end_error_state);
}

auto statement_while_expr_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_open(ctx.current()));
  ctx.push_states(statement_while_expr_end_state, statement_unexpected_end_error_state);
  return ctx.move_next_state(expr_start_state, statement_unexpected_end_error_state);
}

auto statement_while_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_while(ctx.current()));
  ctx.syntax_stack.emplace(std::make_unique<WhileStatement>(ctx.current().pos()));
  return ctx.move_next_state(is_paren_open,
                             statement_while_expr_start_state,
                             statement_while_error_state,
                             statement_unexpected_end_error_state);
}

/*
 * For Statement
 */

auto statement_for_error_handler_(ParserContext& ctx) -> ParserState;
auto statement_for_body_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_for_body_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_for_postfix_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_for_condition_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_for_prefix_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_for_prefix_start_handler_(ParserContext& ctx) -> ParserState;
auto statement_for_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_for_error_state       = ParserState{statement_for_error_handler_};
constexpr auto statement_for_body_end_state    = ParserState{statement_for_body_end_handler_};
constexpr auto statement_for_body_start_state  = ParserState{statement_for_body_start_handler_};
constexpr auto statement_for_postfix_end_state = ParserState{statement_for_postfix_end_handler_};
constexpr auto statement_for_condition_end_state =
    ParserState{statement_for_condition_end_handler_};
constexpr auto statement_for_prefix_end_state   = ParserState{statement_for_prefix_end_handler_};
constexpr auto statement_for_prefix_start_state = ParserState{statement_for_prefix_start_handler_};
constexpr auto statement_for_state              = ParserState{statement_for_handler_};

auto statement_for_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto statement_for_body_end_handler_(ParserContext& ctx) -> ParserState {
  append_body_to_statement(ctx);
  return ctx.pop_ret_state();
}

auto statement_for_body_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_open(ctx.current()));
  ctx.push_states(statement_for_body_end_state, statement_unexpected_end_error_state);
  return statement_block_start_state;
}

auto statement_for_postfix_end_handler_(ParserContext& ctx) -> ParserState {
  auto expr = ctx.pop_syntax_node<BaseExpression>();
  ctx.get_syntax_node<ForStatement>().set_postfix(std::move(expr));

  return ctx.move_next_state(is_curly_open,
                             statement_for_body_start_state,
                             statement_for_error_state,
                             statement_unexpected_end_error_state);
}

auto statement_for_condition_end_handler_(ParserContext& ctx) -> ParserState {
  assert(is_semicolon(ctx.current()));
  auto expr = ctx.pop_syntax_node<BaseExpression>();
  ctx.get_syntax_node<ForStatement>().set_cond(std::move(expr));

  ctx.push_states(statement_for_postfix_end_state, statement_unexpected_end_error_state);
  return ctx.move_next_state(expr_start_state, statement_unexpected_end_error_state);
}

auto statement_for_prefix_end_handler_(ParserContext& ctx) -> ParserState {
  auto expr = ctx.pop_syntax_node<BaseStatement>();
  ctx.get_syntax_node<ForStatement>().set_prefix(std::move(expr));

  ctx.push_states(statement_for_condition_end_state, statement_unexpected_end_error_state);
  return expr_start_state;
}

auto statement_for_prefix_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_paren_open(ctx.current()));
  ctx.push_states(statement_for_prefix_end_state, statement_unexpected_end_error_state);
  return ctx.move_next_state(statement_start_state, statement_unexpected_end_error_state);
}

auto statement_for_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_for(ctx.current()));
  ctx.syntax_stack.emplace(std::make_unique<ForStatement>(ctx.current().pos()));
  return ctx.move_next_state(is_paren_open,
                             statement_for_prefix_start_state,
                             statement_for_error_state,
                             statement_unexpected_end_error_state);
}

/*
 * Statement Start
 */

auto statement_unknown_handler_(ParserContext& ctx) -> ParserState;
static constexpr ParserState statement_unknown_state = ParserState{statement_unknown_handler_};

auto statement_unknown_handler_(ParserContext& ctx) -> ParserState {
  switch (ctx.current().kind()) {
    case LexicalKind::KeywordVar:
      return statement_def_var_state;
    case LexicalKind::KeywordReturn:
      return statement_return_state;
    case LexicalKind::KeywordIf:
      return statement_if_state;
    case LexicalKind::KeywordElif:
      return statement_elif_state;
    case LexicalKind::KeywordElse:
      return statement_else_state;
    case LexicalKind::KeywordLoop:
      return statement_loop_state;
    case LexicalKind::KeywordWhile:
      return statement_while_state;
    case LexicalKind::KeywordFor:
      return statement_for_state;
    case LexicalKind::KeywordForeach:
      throw_not_implemented();
    case LexicalKind::SymbolSemicolon:
      throw_not_implemented();
    default:
      return statement_expr_state;
  }

  throw_not_implemented();
}

constexpr ParserState statement_start_state = ParserState{statement_unknown_handler_};

/*
 * Statement Block
 */

auto statement_block_error_handler_(ParserContext& ctx) -> ParserState;
auto statement_block_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState;
auto statement_block_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_block_statement_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_block_possible_end_handler_(ParserContext& ctx) -> ParserState;
auto statement_block_statement_handler_(ParserContext& ctx) -> ParserState;
auto statement_block_start_handler_(ParserContext& ctx) -> ParserState;

constexpr auto statement_block_error_state = ParserState{statement_block_error_handler_};
constexpr auto statement_block_unexpected_end_error_state =
    ParserState{statement_block_unexpected_end_error_handler_};
constexpr auto statement_block_end_state = ParserState{statement_block_end_handler_};
constexpr auto statement_block_statement_end_state =
    ParserState{statement_block_statement_end_handler_};
constexpr auto statement_block_statement_state = ParserState{statement_block_statement_handler_};
constexpr ParserState statement_block_possible_end_state =
    ParserState{statement_block_possible_end_handler_};
constexpr ParserState statement_block_start_state = ParserState{statement_block_start_handler_};

auto statement_block_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto statement_block_unexpected_end_error_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

auto statement_block_end_handler_(ParserContext& ctx) -> ParserState {
  return ctx.move_next_stack();
}

auto statement_block_statement_end_handler_(ParserContext& ctx) -> ParserState {
  auto statement = ctx.pop_syntax_node<BaseStatement>();
  ctx.get_syntax_node<StatementBlock>().push_statement(std::move(statement));
  return statement_block_possible_end_state;
}

auto statement_block_statement_handler_(ParserContext& ctx) -> ParserState {
  ctx.push_states(statement_block_statement_end_state, statement_block_unexpected_end_error_state);
  return statement_start_state;
}

auto statement_block_possible_end_handler_(ParserContext& ctx) -> ParserState {
  if (is_curly_close(ctx.current())) {
    return statement_block_end_state;
  } else {
    return statement_block_statement_state;
  }
}

auto statement_block_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_open(ctx.current()));

  ctx.syntax_stack.push(std::make_unique<StatementBlock>(ctx.current().pos()));
  return ctx.move_next_state(statement_block_possible_end_state,
                             statement_block_unexpected_end_error_state);
}
