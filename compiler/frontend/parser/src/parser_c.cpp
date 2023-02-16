// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_c.hpp"

/*
 * C Include
 */

auto cinclude_error_handler_(ParserContext& ctx) -> ParserState;

auto cinclude_end_handler_(ParserContext& ctx) -> ParserState;
auto cinclude_name_handler_(ParserContext& ctx) -> ParserState;
auto cinclude_start_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState cinclude_error_state = ParserState{cinclude_error_handler_};

constexpr ParserState cinclude_end_state   = ParserState{cinclude_end_handler_};
constexpr ParserState cinclude_name_state  = ParserState{cinclude_name_handler_};
constexpr ParserState cinclude_start_state = ParserState{cinclude_start_handler_};

auto cinclude_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto cinclude_end_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_semicolon(current));
  return ctx.move_next_stack();
}

auto cinclude_name_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_string(current));
  ctx.get_syntax_node<CInclude>().set_name(current.value());
  return ctx.move_next_state(
      is_semicolon, cinclude_end_state, cinclude_error_state, cinclude_error_state);
}

auto cinclude_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_keyword_cinclude(current));
  ctx.syntax_stack.emplace(std::make_unique<CInclude>(current.pos()));
  return ctx.move_next_state(
      is_string, cinclude_name_state, cinclude_error_state, cinclude_error_state);
}

/*
 * C Type
 */

auto ctype_error_handler_(ParserContext& ctx) -> ParserState;

auto ctype_end_handler_(ParserContext& ctx) -> ParserState;
auto ctype_cname_handler_(ParserContext& ctx) -> ParserState;
auto ctype_colon_handler_(ParserContext& ctx) -> ParserState;
auto ctype_name_handler_(ParserContext& ctx) -> ParserState;
auto ctype_start_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState ctype_error_state = ParserState{ctype_error_handler_};

constexpr ParserState ctype_end_state   = ParserState{ctype_end_handler_};
constexpr ParserState ctype_cname_state = ParserState{ctype_cname_handler_};
constexpr ParserState ctype_colon_state = ParserState{ctype_colon_handler_};
constexpr ParserState ctype_name_state  = ParserState{ctype_name_handler_};
constexpr ParserState ctype_start_state = ParserState{ctype_start_handler_};

auto ctype_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto ctype_end_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_semicolon(current));
  return ctx.move_next_stack();
}

auto ctype_cname_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_string(current));
  ctx.get_syntax_node<CTypeDefinition>().set_c_name(current.value());
  return ctx.move_next_state(is_semicolon, ctype_end_state, ctype_error_state, ctype_error_state);
}

auto ctype_colon_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_colon(current));
  return ctx.move_next_state(is_string, ctype_cname_state, ctype_error_state, ctype_error_state);
}

auto ctype_name_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_identifier(current));
  ctx.get_syntax_node<CTypeDefinition>().set_name(current.value());
  return ctx.move_next_state(is_colon, ctype_colon_state, ctype_error_state, ctype_error_state);
}

auto ctype_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_keyword_ctype(current));
  ctx.syntax_stack.emplace(std::make_unique<CTypeDefinition>(current.pos()));
  return ctx.move_next_state(is_identifier, ctype_name_state, ctype_error_state, ctype_error_state);
}