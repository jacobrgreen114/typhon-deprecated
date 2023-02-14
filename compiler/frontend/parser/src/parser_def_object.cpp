// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_def_object.hpp"

#include "parser_def_var.hpp"
#include "parser_def_func.hpp"
#include "parser_def_struct.hpp"

auto def_object_error_handler_(ParserContext& ctx) -> ParserState;
auto def_object_unexpected_end_handler_(ParserContext& ctx) -> ParserState;

auto def_object_body_end_handler_(ParserContext& ctx) -> ParserState;
auto def_object_body_poss_end_handler_(ParserContext& ctx) -> ParserState;

auto def_object_var_end_handler_(ParserContext& ctx) -> ParserState;
auto def_object_func_end_handler_(ParserContext& ctx) -> ParserState;
auto def_object_struct_end_handler_(ParserContext& ctx) -> ParserState;
auto def_object_object_end_handler_(ParserContext& ctx) -> ParserState;

auto def_object_body_handler_(ParserContext& ctx) -> ParserState;
auto def_object_body_start_handler_(ParserContext& ctx) -> ParserState;

auto def_object_name_handler_(ParserContext& ctx) -> ParserState;
auto def_object_handler_(ParserContext& ctx) -> ParserState;

constexpr auto def_object_error_state          = ParserState{def_object_error_handler_};
constexpr auto def_object_unexpected_end_state = ParserState{def_object_unexpected_end_handler_};
constexpr auto def_object_body_end_state       = ParserState{def_object_body_end_handler_};

constexpr auto def_object_body_poss_end_state  = ParserState{def_object_body_poss_end_handler_};

constexpr auto def_object_var_end_state        = ParserState{def_object_var_end_handler_};
constexpr auto def_object_func_end_state       = ParserState{def_object_func_end_handler_};
constexpr auto def_object_struct_end_state     = ParserState{def_object_struct_end_handler_};
constexpr auto def_object_object_end_state     = ParserState{def_object_object_end_handler_};

constexpr auto def_object_body_state           = ParserState{def_object_body_handler_};
constexpr auto def_object_body_start_state     = ParserState{def_object_body_start_handler_};
constexpr auto def_object_name_state           = ParserState{def_object_name_handler_};
constexpr ParserState def_object_start_state   = ParserState{def_object_handler_};

auto def_object_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto def_object_unexpected_end_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

auto def_object_body_end_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_close(ctx.current()));
  return ctx.move_next_stack();
}

auto def_object_body_poss_end_handler_(ParserContext& ctx) -> ParserState {
  if (is_curly_close(ctx.current())) {
    return def_object_body_end_state;
  }
  return def_object_body_state;
}

auto def_object_var_end_handler_(ParserContext& ctx) -> ParserState {
  auto var = ctx.pop_syntax_node<VariableDefinition>();
  ctx.get_syntax_node<ObjectDefinition>().push_var(std::move(var));
  return def_object_body_poss_end_state;
}

auto def_object_func_end_handler_(ParserContext& ctx) -> ParserState {
  auto func = ctx.pop_syntax_node<FunctionDefinition>();
  ctx.get_syntax_node<ObjectDefinition>().push_func(std::move(func));
  return def_object_body_poss_end_state;
}

auto def_object_struct_end_handler_(ParserContext& ctx) -> ParserState {
  auto strct = ctx.pop_syntax_node<StructDefinition>();
  ctx.get_syntax_node<ObjectDefinition>().push_struct(std::move(strct));
  return def_object_body_poss_end_state;
}

auto def_object_object_end_handler_(ParserContext& ctx) -> ParserState {
  auto object = ctx.pop_syntax_node<ObjectDefinition>();
  ctx.get_syntax_node<ObjectDefinition>().push_object(std::move(object));
  return def_object_body_poss_end_state;
}

auto def_object_body_handler_(ParserContext& ctx) -> ParserState {
  switch (ctx.current().kind()) {
    case LexicalKind::KeywordVar:
      ctx.push_states(def_object_var_end_state, def_object_unexpected_end_state);
      return var_def_start_state;
    case LexicalKind::KeywordFunc:
      ctx.push_states(def_object_func_end_state, def_object_unexpected_end_state);
      return func_def_start_state;
    case LexicalKind::KeywordStruct:
      ctx.push_states(def_object_struct_end_state, def_object_unexpected_end_state);
      return def_struct_start_state;
    case LexicalKind::KeywordObject:
      ctx.push_states(def_object_object_end_state, def_object_unexpected_end_state);
      return def_object_start_state;
    default:
      return def_object_error_state;
  }
}

auto def_object_body_start_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_curly_open(current));
  return ctx.move_next_state(is_curly_close,
                             def_object_body_end_state,
                             def_object_body_state,
                             def_object_unexpected_end_state);
}

auto def_object_name_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_identifier(current));
  ctx.get_syntax_node<ObjectDefinition>().set_name(current.value());

  return ctx.move_next_state(is_curly_open,
                             def_object_body_start_state,
                             def_object_error_state,
                             def_object_unexpected_end_state);
}

auto def_object_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_object(ctx.current()));
  ctx.syntax_stack.push(std::make_unique<ObjectDefinition>(ctx.current().pos()));
  return ctx.move_next_state(is_identifier,
                             def_object_name_state,
                             def_object_error_state,
                             def_object_unexpected_end_state);
}