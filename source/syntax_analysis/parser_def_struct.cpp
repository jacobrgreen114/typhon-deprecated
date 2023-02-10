// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_def_struct.hpp"

#include "parser_def_var.hpp"
#include "parser_def_func.hpp"

auto def_struct_error_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_unexpected_end_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_body_end_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_body_poss_end_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_body_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_body_start_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_name_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_handler_(ParserContext& ctx) -> ParserState;

constexpr auto def_struct_error_state          = ParserState{def_struct_error_handler_};
constexpr auto def_struct_unexpected_end_state = ParserState{def_struct_unexpected_end_handler_};
constexpr auto def_struct_body_end_state       = ParserState{def_struct_body_end_handler_};

constexpr auto def_struct_body_poss_end_state  = ParserState{def_struct_body_poss_end_handler_};
constexpr auto def_struct_body_state           = ParserState{def_struct_body_handler_};
constexpr auto def_struct_body_start_state     = ParserState{def_struct_body_start_handler_};
constexpr auto def_struct_name_state           = ParserState{def_struct_name_handler_};
constexpr ParserState def_struct_state         = ParserState{def_struct_handler_};

auto def_struct_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto def_struct_unexpected_end_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

auto def_struct_body_end_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_bracket_close(current));
  return ctx.move_next_stack();
}

auto def_struct_body_poss_end_handler_(ParserContext& ctx) -> ParserState {
  auto def = ctx.pop_syntax_node<Definition>();
  auto str = ctx.get_syntax_node<StructDefinition>();
  str->push_definition(def);

  if (is_bracket_close(ctx.current())) {
    return def_struct_body_end_state;
  }
  return def_struct_body_state;
}

auto def_struct_body_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();

  ctx.push_states(def_struct_body_poss_end_state, def_struct_unexpected_end_state);
  switch (current.kind()) {
    case LexicalKind::KeywordVar:
      return var_def_start_state;
    case LexicalKind::KeywordFunc:
      return func_def_start_state;
    case LexicalKind::KeywordStruct:
      return def_struct_state;
    default:
      return def_struct_error_state;
  }

  throw_not_implemented();
}

auto def_struct_body_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_bracket_open(current));
  return ctx.move_next_state(is_bracket_close,
                             def_struct_body_end_state,
                             def_struct_body_state,
                             def_struct_unexpected_end_state);
}

auto def_struct_name_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_identifier(current));
  auto& name = current.value();

  auto def   = ctx.get_syntax_node<StructDefinition>();
  def->set_name(name);

  return ctx.move_next_state(is_bracket_open,
                             def_struct_body_start_state,
                             def_struct_error_state,
                             def_struct_unexpected_end_state);
}

auto def_struct_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_struct(ctx.current()));
  ctx.syntax_stack.push(std::make_shared<StructDefinition>());
  return ctx.move_next_state(is_identifier,
                             def_struct_name_state,
                             def_struct_error_state,
                             def_struct_unexpected_end_state);
}
