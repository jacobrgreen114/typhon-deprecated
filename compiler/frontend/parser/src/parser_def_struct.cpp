// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_def_struct.hpp"

#include "parser_def_var.hpp"
#include "parser_def_func.hpp"
#include "parser_def_object.hpp"

auto def_struct_error_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_unexpected_end_handler_(ParserContext& ctx) -> ParserState;

auto def_struct_body_end_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_body_poss_end_handler_(ParserContext& ctx) -> ParserState;

auto def_struct_var_end_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_func_end_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_struct_end_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_object_end_handler_(ParserContext& ctx) -> ParserState;

auto def_struct_body_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_body_start_handler_(ParserContext& ctx) -> ParserState;

auto def_struct_name_handler_(ParserContext& ctx) -> ParserState;
auto def_struct_handler_(ParserContext& ctx) -> ParserState;

constexpr auto def_struct_error_state          = ParserState{def_struct_error_handler_};
constexpr auto def_struct_unexpected_end_state = ParserState{def_struct_unexpected_end_handler_};

constexpr auto def_struct_body_end_state       = ParserState{def_struct_body_end_handler_};
constexpr auto def_struct_body_poss_end_state  = ParserState{def_struct_body_poss_end_handler_};

constexpr auto def_struct_var_end_state        = ParserState{def_struct_var_end_handler_};
constexpr auto def_struct_func_end_state       = ParserState{def_struct_func_end_handler_};
constexpr auto def_struct_struct_end_state     = ParserState{def_struct_struct_end_handler_};
constexpr auto def_struct_object_end_state     = ParserState{def_struct_object_end_handler_};

constexpr auto def_struct_body_state           = ParserState{def_struct_body_handler_};
constexpr auto def_struct_body_start_state     = ParserState{def_struct_body_start_handler_};

constexpr auto def_struct_name_state           = ParserState{def_struct_name_handler_};
constexpr ParserState def_struct_start_state   = ParserState{def_struct_handler_};

auto def_struct_error_handler_(ParserContext& ctx) -> ParserState { throw_not_implemented(); }

auto def_struct_unexpected_end_handler_(ParserContext& ctx) -> ParserState {
  throw_not_implemented();
}

auto def_struct_body_end_handler_(ParserContext& ctx) -> ParserState {
  auto& current = ctx.current();
  assert(is_curly_close(current));
  return ctx.move_next_stack();
}

auto def_struct_body_poss_end_handler_(ParserContext& ctx) -> ParserState {
  if (is_curly_close(ctx.current())) {
    return def_struct_body_end_state;
  }
  return def_struct_body_state;
}

template <typename T>
auto do_type_end_handler(ParserContext& ctx) -> ParserState {
  auto var = ctx.pop_syntax_node<VariableDefinition>();
  ctx.get_syntax_node<StructDefinition>().push_var(std::move(var));
  return def_struct_body_poss_end_state;
}

auto def_struct_var_end_handler_(ParserContext& ctx) -> ParserState {
  auto var = ctx.pop_syntax_node<VariableDefinition>();
  ctx.get_syntax_node<StructDefinition>().push_var(std::move(var));
  return def_struct_body_poss_end_state;
}

auto def_struct_func_end_handler_(ParserContext& ctx) -> ParserState {
  auto func = ctx.pop_syntax_node<FunctionDefinition>();
  ctx.get_syntax_node<StructDefinition>().push_func(std::move(func));
  return def_struct_body_poss_end_state;
}

auto def_struct_struct_end_handler_(ParserContext& ctx) -> ParserState {
  auto strct = ctx.pop_syntax_node<StructDefinition>();
  ctx.get_syntax_node<StructDefinition>().push_struct(std::move(strct));
  return def_struct_body_poss_end_state;
}

auto def_struct_object_end_handler_(ParserContext& ctx) -> ParserState {
  auto object = ctx.pop_syntax_node<ObjectDefinition>();
  ctx.get_syntax_node<StructDefinition>().push_object(std::move(object));
  return def_struct_body_poss_end_state;
}

auto def_struct_body_handler_(ParserContext& ctx) -> ParserState {
  switch (ctx.current().kind()) {
    case LexicalKind::KeywordVar:
      ctx.push_states(def_struct_var_end_state, def_struct_unexpected_end_state);
      return var_def_start_state;
    case LexicalKind::KeywordFunc:
      ctx.push_states(def_struct_func_end_state, def_struct_unexpected_end_state);
      return func_def_start_state;
    case LexicalKind::KeywordStruct:
      ctx.push_states(def_struct_struct_end_state, def_struct_unexpected_end_state);
      return def_struct_start_state;
    case LexicalKind::KeywordObject:
      ctx.push_states(def_struct_object_end_state, def_struct_unexpected_end_state);
      return def_object_start_state;
    default:
      return def_struct_error_state;
  }

  throw_not_implemented();
}

auto def_struct_body_start_handler_(ParserContext& ctx) -> ParserState {
  assert(is_curly_open(ctx.current()));
  return ctx.move_next_state(is_curly_close,
                             def_struct_body_end_state,
                             def_struct_body_state,
                             def_struct_unexpected_end_state);
}

auto def_struct_name_handler_(ParserContext& ctx) -> ParserState {
  assert(is_identifier(ctx.current()));
  ctx.get_syntax_node<StructDefinition>().set_name(ctx.current().value());

  return ctx.move_next_state(is_curly_open,
                             def_struct_body_start_state,
                             def_struct_error_state,
                             def_struct_unexpected_end_state);
}

auto def_struct_handler_(ParserContext& ctx) -> ParserState {
  assert(is_keyword_struct(ctx.current()));
  ctx.syntax_stack.push(std::make_unique<StructDefinition>(ctx.current().pos()));
  auto& def = ctx.get_syntax_node<StructDefinition>();

  while (!ctx.token_stack.empty()) {
    auto token = ctx.pop_token_stack();
    if (auto opt_acc_mod = get_access_modifier(token.kind()); opt_acc_mod.has_value()) {
      def.set_access(opt_acc_mod.value());
    }
    else {
      throw_not_implemented();
    }
  }

  return ctx.move_next_state(is_identifier,
                             def_struct_name_state,
                             def_struct_error_state,
                             def_struct_unexpected_end_state);
}
