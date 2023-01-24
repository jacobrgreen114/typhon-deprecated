// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "fndef_parsing.hpp"

#include "statement_parsing.hpp"

auto get_func_def_syntax_node(ParserContext& ctx) -> FuncDefinition* {
  auto& p_syntax = ctx.syntax_stack.top();
  assert(p_syntax && p_syntax->kind() == SyntaxKind::DefFunc);
  return static_cast<FuncDefinition*>(p_syntax.get());
}

auto get_func_def_param_syntax_node(ParserContext& ctx) -> FuncParameter* {
  auto& p_syntax = ctx.syntax_stack.top();
  assert(p_syntax && p_syntax->kind() == SyntaxKind::DefParam);
  return static_cast<FuncParameter*>(p_syntax.get());
}

auto pop_func_def_param_syntax_node(ParserContext& ctx)
    -> std::shared_ptr<FuncParameter> {
  auto p_syntax = std::move(ctx.syntax_stack.top());
  ctx.syntax_stack.pop();
  assert(p_syntax && p_syntax->kind() == SyntaxKind::DefParam);
  return static_pointer_cast<FuncParameter>(p_syntax);
}

static constexpr ParserState func_def_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState func_def_unexpected_end_error_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState func_def_body_end_exit_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState func_def_body_end_state = ParserState{
    [](ParserContext& ctx) -> ParserState { throw_not_implemented(); }};

static constexpr ParserState func_def_body_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_brace_open(current));
      ctx.push_states(func_def_body_end_state, func_def_body_end_exit_state);
      return statement_block_start_state;
    }};

static constexpr ParserState func_def_return_name_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_identifier(current));

      auto* func_def = get_func_def_syntax_node(ctx);
      func_def->set_return_type(current.value());

      return ctx.move_next_state(is_brace_open, func_def_body_start_state,
                                 func_def_error_state,
                                 func_def_unexpected_end_error_state);
    }};

static constexpr ParserState func_def_return_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_arrow(current));
      return ctx.move_next_state(is_identifier, func_def_return_name_state,
                                 func_def_error_state,
                                 func_def_unexpected_end_error_state);
    }};

static constexpr ParserState func_def_param_block_end_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_paren_close(current));

      static constexpr auto conditions =
          std::array<ParserContext::RefMatchCondition, 2>{
              ParserContext::RefMatchCondition{is_arrow,
                                               func_def_return_start_state},
              ParserContext::RefMatchCondition{is_brace_open,
                                               func_def_body_start_state}};

      return ctx.move_next_state(func_def_error_state,
                                 func_def_unexpected_end_error_state,
                                 conditions);
      throw_not_implemented();
    }};

extern const ParserState func_def_param_start_state;

static constexpr ParserState func_def_param_next_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_comma(current));
      return ctx.move_next_state(is_identifier, func_def_param_start_state,
                                 func_def_error_state,
                                 func_def_unexpected_end_error_state);
    }};

static constexpr ParserState func_def_param_end_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto param = pop_func_def_param_syntax_node(ctx);
      auto* func_def = get_func_def_syntax_node(ctx);
      func_def->push_parameter(param);

      static constexpr auto conditions =
          std::array<ParserContext::RefMatchCondition, 2>{
              ParserContext::RefMatchCondition{is_comma,
                                               func_def_param_next_state},
              ParserContext::RefMatchCondition{is_paren_close,
                                               func_def_param_block_end_state}};

      return ctx.move_next_state(func_def_error_state,
                                 func_def_unexpected_end_error_state,
                                 conditions);
    }};

static constexpr ParserState func_def_param_type_name_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_identifier(current));

      auto* param_def = get_func_def_param_syntax_node(ctx);
      param_def->set_type_name(current.value());

      return func_def_param_end_state;
    }};

static constexpr ParserState func_def_param_type_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_colon(current));
      return ctx.move_next_state(is_identifier, func_def_param_type_name_state,
                                 func_def_error_state,
                                 func_def_unexpected_end_error_state);
    }};

static constexpr ParserState func_def_param_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_identifier(current));
      ctx.syntax_stack.push(std::make_shared<FuncParameter>(current.value()));

      static constexpr auto conditions =
          std::array<ParserContext::RefMatchCondition, 1>{
              ParserContext::RefMatchCondition{
                  is_colon, func_def_param_type_start_state}};

      return ctx.move_next_state(func_def_param_end_state,
                                 func_def_unexpected_end_error_state,
                                 conditions);
    }};

// func main [ ( ] ) -> int { return 0; }
static constexpr ParserState func_def_param_block_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_paren_open(ctx.current()));

      static constexpr auto conditions =
          std::array<ParserContext::RefMatchCondition, 2>{
              ParserContext::RefMatchCondition{is_identifier,
                                               func_def_param_start_state},
              ParserContext::RefMatchCondition{is_paren_close,
                                               func_def_param_block_end_state}};
      return ctx.move_next_state(func_def_error_state,
                                 func_def_unexpected_end_error_state,
                                 conditions);
    }};

// func [ main ] ( ) -> int { return 0; }
static constexpr ParserState func_def_identifier_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      assert(is_identifier(ctx.current()));

      auto* func_def = get_func_def_syntax_node(ctx);
      auto identifier_name = ctx.current().value();
      func_def->set_name(identifier_name);

      return ctx.move_next_state(
          is_paren_open, func_def_param_block_start_state, func_def_error_state,
          func_def_unexpected_end_error_state);
    }};

// [ func ] main () -> int { return 0; }
static constexpr ParserState func_def_start_state =
    ParserState{[](ParserContext& ctx) -> ParserState {
      auto& current = ctx.current();
      assert(is_keyword_func(current));
      ctx.syntax_stack.emplace(std::make_shared<FuncDefinition>());
      return ctx.move_next_state(is_identifier, func_def_identifier_state,
                                 func_def_error_state,
                                 func_def_unexpected_end_error_state);
    }};