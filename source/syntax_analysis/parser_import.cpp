// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_import.hpp"

auto import_error_handler_(ParserContext& ctx) -> ParserState;

auto import_end_handler_(ParserContext& ctx) -> ParserState;
auto import_identifier_next_handler_(ParserContext& ctx) -> ParserState;
auto import_identifier_handler_(ParserContext& ctx) -> ParserState;
auto import_start_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState import_error_state           = ParserState{import_error_handler_};
constexpr ParserState import_unexpected_end_state  = ParserState{import_error_handler_};

constexpr ParserState import_end_state             = ParserState{import_end_handler_};
constexpr ParserState import_identifier_next_state = ParserState{import_identifier_next_handler_};
constexpr ParserState import_identifier_state      = ParserState{import_identifier_handler_};
constexpr ParserState import_start_state           = ParserState{import_start_handler_};

auto import_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto import_end_handler_(ParserContext& ctx) -> ParserState {
  assert(is_semicolon(ctx.current()));
  return ctx.move_next_stack();
}

auto import_identifier_next_handler_(ParserContext& ctx) -> ParserState {
  assert(is_doublecolon(ctx.current()));
  return ctx.move_next_state(
      is_identifier, import_identifier_state, import_error_state, import_unexpected_end_state);
}

auto import_identifier_handler_(ParserContext& ctx) -> ParserState {
  assert(is_identifier(ctx.current()));
  auto& value = ctx.current().value();
  auto node   = ctx.get_syntax_node<Import>();
  node->push_namespace(value);

  static constexpr auto conditions = std::array<ParserMatchCondition, 2>{
      ParserMatchCondition{is_doublecolon, import_identifier_next_state},
      ParserMatchCondition{is_semicolon,   import_end_state            }
  };

  return ctx.move_next_state(error_state, import_unexpected_end_state, conditions);
}

auto import_start_handler_(ParserContext& ctx) -> ParserState {
  assert(ctx.current().kind() == LexicalKind::KeywordImport);
  ctx.syntax_stack.emplace(std::make_shared<Import>());
  return ctx.move_next_state(
      is_identifier, import_identifier_state, import_error_state, import_unexpected_end_state);
}
