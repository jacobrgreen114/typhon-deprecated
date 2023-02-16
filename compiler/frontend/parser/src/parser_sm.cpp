// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "parser_sm.hpp"

#include <utility>

#include "parser_import.hpp"
#include "parser_def_var.hpp"
#include "parser_def_func.hpp"
#include "parser_def_struct.hpp"
#include "parser_def_object.hpp"
#include "parser_c.hpp"

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

auto is_def_modifier(const LexicalToken& token) -> bool {
  auto kind = token.kind();
  switch (kind) {
    case LexicalKind::KeywordPrivate:
    case LexicalKind::KeywordInternal:
    case LexicalKind::KeywordProtected:
    case LexicalKind::KeywordPublic:
      return true;

    default:
      return false;
  }
}

#pragma region Global Parser States

auto error_handler_(ParserContext& ctx) -> ParserState;
auto unexpected_token_error_handler_(ParserContext& ctx) -> ParserState;
auto end_handler_(ParserContext& ctx) -> ParserState;

auto source_import_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_import_end_handler_(ParserContext& ctx) -> ParserState;

auto source_ns_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_ns_end_handler_(ParserContext& ctx) -> ParserState;

auto source_var_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_var_end_handler_(ParserContext& ctx) -> ParserState;

auto source_func_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_func_end_handler_(ParserContext& ctx) -> ParserState;

auto source_struct_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_struct_end_handler_(ParserContext& ctx) -> ParserState;

auto source_object_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_object_end_handler_(ParserContext& ctx) -> ParserState;

auto source_cinclude_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_cinclude_end_handler_(ParserContext& ctx) -> ParserState;

auto source_ctype_end_eof_handler_(ParserContext& ctx) -> ParserState;
auto source_ctype_end_handler_(ParserContext& ctx) -> ParserState;

auto unknown_handler(ParserContext& ctx) -> ParserState;

auto start_handler_(ParserContext& ctx) -> ParserState;

constexpr ParserState error_state                  = ParserState{error_handler_};
constexpr ParserState unexpected_token_error_state = ParserState{unexpected_token_error_handler_};
constexpr auto exit_state                          = ParserState{nullptr};

constexpr auto source_import_end_eof_state         = ParserState{source_import_end_eof_handler_};
constexpr auto source_import_end_state             = ParserState{source_import_end_handler_};

constexpr auto source_ns_end_eof_state             = ParserState{source_ns_end_eof_handler_};
constexpr auto source_ns_end_state                 = ParserState{source_ns_end_handler_};

constexpr auto source_var_end_eof_state            = ParserState{source_var_end_eof_handler_};
constexpr auto source_var_end_state                = ParserState{source_var_end_handler_};

constexpr auto source_func_end_eof_state           = ParserState{source_func_end_eof_handler_};
constexpr auto source_func_end_state               = ParserState{source_func_end_handler_};

constexpr auto source_struct_end_eof_state         = ParserState{source_struct_end_eof_handler_};
constexpr auto source_struct_end_state             = ParserState{source_struct_end_handler_};

constexpr auto source_object_end_eof_state         = ParserState{source_object_end_eof_handler_};
constexpr auto source_object_end_state             = ParserState{source_object_end_handler_};

constexpr auto source_cinclude_end_eof_state       = ParserState{source_cinclude_end_eof_handler_};
constexpr auto source_cinclude_end_state           = ParserState{source_cinclude_end_handler_};

constexpr auto source_ctype_end_eof_state          = ParserState{source_ctype_end_eof_handler_};
constexpr auto source_ctype_end_state              = ParserState{source_ctype_end_handler_};

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

auto source_import_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_import(ctx.pop_syntax_node<NamespaceImport>());
  return exit_state;
}

auto source_import_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_import(ctx.pop_syntax_node<NamespaceImport>());
  return unknown_state;
}

auto source_ns_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_namespace(ctx.pop_syntax_node<NamespaceDeclaration>());
  return exit_state;
}

auto source_ns_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_namespace(ctx.pop_syntax_node<NamespaceDeclaration>());
  return unknown_state;
}

auto source_var_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_var(ctx.pop_syntax_node<VariableDefinition>());
  return exit_state;
}

auto source_var_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_var(ctx.pop_syntax_node<VariableDefinition>());
  return unknown_state;
}

auto source_func_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_func(ctx.pop_syntax_node<FunctionDefinition>());
  return exit_state;
}

auto source_func_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_func(ctx.pop_syntax_node<FunctionDefinition>());
  return unknown_state;
}

auto source_struct_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_struct(ctx.pop_syntax_node<StructDefinition>());
  return exit_state;
}

auto source_struct_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_struct(ctx.pop_syntax_node<StructDefinition>());
  return unknown_state;
}

auto source_object_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_object(ctx.pop_syntax_node<ObjectDefinition>());
  return exit_state;
}

auto source_object_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_object(ctx.pop_syntax_node<ObjectDefinition>());
  return unknown_state;
}

auto source_cinclude_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_cinclude(ctx.pop_syntax_node<CInclude>());
  return exit_state;
}

auto source_cinclude_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_cinclude(ctx.pop_syntax_node<CInclude>());
  return unknown_state;
}

auto source_ctype_end_eof_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_ctype(ctx.pop_syntax_node<CTypeDefinition>());
  return exit_state;
}

auto source_ctype_end_handler_(ParserContext& ctx) -> ParserState {
  ctx.source->push_ctype(ctx.pop_syntax_node<CTypeDefinition>());
  return unknown_state;
}

auto unknown_handler(ParserContext& ctx) -> ParserState {
  if (is_def_modifier(ctx.current())) {
    ctx.push_current_token();
  }

  auto kind = ctx.current().kind();
  switch (kind) {
    case LexicalKind::KeywordVar: {
      ctx.push_states(source_var_end_state, source_var_end_eof_state);
      return var_def_start_state;
    }
    case LexicalKind::KeywordFunc: {
      ctx.push_states(source_func_end_state, source_func_end_eof_state);
      return func_def_start_state;
    }
    case LexicalKind::KeywordStruct: {
      ctx.push_states(source_struct_end_state, source_struct_end_eof_state);
      return def_struct_start_state;
    }
    case LexicalKind::KeywordObject: {
      ctx.push_states(source_object_end_state, source_object_end_eof_state);
      return def_object_start_state;
    }
    case LexicalKind::KeywordImport: {
      ctx.push_states(source_import_end_state, source_import_end_eof_state);
      return import_start_state;
    }
    case LexicalKind::KeywordNamespace: {
      ctx.push_states(source_ns_end_state, source_ns_end_eof_state);
      return namespace_start_state;
    }
    case LexicalKind::KeywordCInclude: {
      ctx.push_states(source_cinclude_end_state, source_cinclude_end_eof_state);
      return cinclude_start_state;
    }
    case LexicalKind::KeywordCType: {
      ctx.push_states(source_ctype_end_state, source_ctype_end_eof_state);
      return ctype_start_state;
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

ParserContext::ParserContext(const TokenCollection& tokens)
    : tokens_{tokens},
      started_{false},
      current_{},
      source{std::make_unique<SyntaxTree>(tokens.source())} {}

auto ParserContext::current() -> const LexicalToken& { return *current_; }

auto ParserContext::move_next() -> bool {
  if (!started_) {
    current_ = tokens_.tokens().begin();
    started_ = true;
    return !(current_ == tokens_.tokens().end());
  }
  return ++current_ < tokens_.tokens().end();
}

#pragma endregion

#pragma region Parser

Parser::Parser()
    : StateMachine<ParserContext>(start_state) {}

#pragma endregion
