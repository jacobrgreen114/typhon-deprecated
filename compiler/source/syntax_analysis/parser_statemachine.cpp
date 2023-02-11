// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_statemachine.hpp"

#include "parser_def_var.hpp"
#include "parser_def_func.hpp"
#include "parser_def_struct.hpp"
#include "parser_import.hpp"

auto is_unary_pre_operator(const LexicalToken& token) -> bool {
  switch (token.kind()) {
    case LexicalKind::SymbolPlus:
    case LexicalKind::SymbolMinus:
    case LexicalKind::SymbolInc:
    case LexicalKind::SymbolDec:
    //case LexicalKind::SymbolStar:
    case LexicalKind::SymbolBoolNot:
    case LexicalKind::SymbolBitNot:
      return true;

    default:
      return false;
  }
};

auto is_unary_post_operator(const LexicalToken& token) -> bool {
  switch (token.kind()) {
    case LexicalKind::SymbolInc:
    case LexicalKind::SymbolDec:
      return true;

    default:
      return false;
  }
};

// todo : reorder kind enum to optimize jump table
auto is_binary_operator(const LexicalToken& token) -> bool {
  auto kind = token.kind();
  switch (kind) {
    case LexicalKind::SymbolPeriod:
    case LexicalKind::SymbolDoubleColon:

    case LexicalKind::SymbolPlus:
    case LexicalKind::SymbolMinus:
    case LexicalKind::SymbolStar:
    case LexicalKind::SymbolSlash:

    case LexicalKind::SymbolEquals:

    case LexicalKind::SymbolBoolEquals:
    case LexicalKind::SymbolBoolNotEquals:
    case LexicalKind::SymbolBoolOr:
    case LexicalKind::SymbolBoolAnd:

    case LexicalKind::SymbolAngleOpen:
    case LexicalKind::SymbolAngleClose:
    case LexicalKind::SymbolLessThanEqual:
    case LexicalKind::SymbolGreaterThanEqual:

    case LexicalKind::SymbolBitOr:
    case LexicalKind::SymbolBitAnd:
    case LexicalKind::SymbolBitXor:

    case LexicalKind::SymbolShiftLeft:
    case LexicalKind::SymbolShiftRight:
      return true;

    default:
      return false;
  }
}

#pragma region Global Parser States

auto error_handler_(ParserContext& ctx) -> ParserState;
auto unexpected_token_error_handler_(ParserContext& ctx) -> ParserState;
auto end_handler_(ParserContext& ctx) -> ParserState;

auto source_node_end_exit_handler_(ParserContext& ctx) -> ParserState;
auto source_node_end_handler_(ParserContext& ctx) -> ParserState;
auto unknown_handler(ParserContext& ctx) -> ParserState;

auto start_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState error_state                  = ParserState{error_handler_};
constexpr ParserState unexpected_token_error_state = ParserState{unexpected_token_error_handler_};
constexpr auto exit_state                          = ParserState{nullptr};
constexpr auto end_state                           = ParserState{end_handler_};

constexpr auto source_node_end_exit_state          = ParserState{source_node_end_exit_handler_};
constexpr auto source_node_end_state               = ParserState{source_node_end_handler_};
constexpr auto unknown_state                       = ParserState{unknown_handler};

constexpr auto start_state                         = ParserState{start_handler_};

auto error_handler_(ParserContext& ctx) -> ParserState {
  std::cerr << "Error at token " << ctx.current() << std::endl;
  assert(false);
  exit(-1);
}

auto unexpected_token_error_handler_(ParserContext& ctx) -> ParserState {
  std::cerr << "Unexpected token " << ctx.current() << std::endl;
  assert(false);
  exit(-1);
}

auto end_handler_(ParserContext& ctx) -> ParserState { return exit_state; }

auto append_source_node(ParserContext& ctx) -> void {
  auto node = std::move(ctx.syntax_stack.top());
  ctx.syntax_stack.pop();
  ctx.source->push_node(node);
  assert(ctx.syntax_stack.empty());
}

auto source_node_end_exit_handler_(ParserContext& ctx) -> ParserState {
  append_source_node(ctx);
  return end_state;
}

auto source_node_end_handler_(ParserContext& ctx) -> ParserState {
  append_source_node(ctx);
  return unknown_state;
}

auto unknown_handler(ParserContext& ctx) -> ParserState {
  auto kind = ctx.current().kind();

  ctx.push_states(source_node_end_state, source_node_end_exit_state);

  switch (kind) {
    case LexicalKind::KeywordVar: {
      return var_def_start_state;
    }
    case LexicalKind::KeywordFunc: {
      return func_def_start_state;
    }
    case LexicalKind::KeywordStruct: {
      return def_struct_state;
    }
    case LexicalKind::KeywordObject: {
      throw_not_implemented();
    }
    case LexicalKind::KeywordImport: {
      return import_start_state;
    }
    case LexicalKind::KeywordNamespace: {
      return namespace_start_state;
    }
    default: {
      return unexpected_token_error_state;
    }
  }
}

auto start_handler_(ParserContext& ctx) -> ParserState {
  return ctx.move_next_state(unknown_state, exit_state);
}

#pragma endregion

#pragma region Parser Context

ParserContext::ParserContext(const std::vector<LexicalToken>& tokens)
    : tokens_{tokens},
      started_{false},
      current_{},
      source{std::make_shared<SyntaxTree>()} {}

auto ParserContext::current() -> const LexicalToken& { return *current_; }

auto ParserContext::move_next() -> bool {
  if (!started_) {
    current_ = tokens_.begin();
    started_ = true;
    return !(current_ == tokens_.end());
  }
  return ++current_ < tokens_.end();
}

#pragma endregion

#pragma region Parser

Parser::Parser()
    : StateMachine<ParserContext>(start_state) {}

#pragma endregion
