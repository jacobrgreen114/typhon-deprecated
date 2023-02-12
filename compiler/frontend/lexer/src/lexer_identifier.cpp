// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "lexer_identifier.hpp"

#define CACHE_IDENTIFIERS true

const auto keywords = std::unordered_map<std::string_view, LexicalKind>{
    {"true",      LexicalKind::KeywordTrue     },
    {"false",     LexicalKind::KeywordFalse    },

    {"var",       LexicalKind::KeywordVar      },
    {"func",      LexicalKind::KeywordFunc     },
    {"struct",    LexicalKind::KeywordStruct   },
    {"object",    LexicalKind::KeywordObject   },

    {"concept",   LexicalKind::KeywordConcept  },
    {"interface", LexicalKind::KeywordInterface},

    {"return",    LexicalKind::KeywordReturn   },

    {"if",        LexicalKind::KeywordIf       },
    {"elif",      LexicalKind::KeywordElif     },
    {"else",      LexicalKind::KeywordElse     },

    {"switch",    LexicalKind::KeywordSwitch   },
    {"match",     LexicalKind::KeywordMatch    },

    {"loop",      LexicalKind::KeywordLoop     },
    {"while",     LexicalKind::KeywordWhile    },
    {"for",       LexicalKind::KeywordFor      },
    {"foreach",   LexicalKind::KeywordForeach  },

    {"namespace", LexicalKind::KeywordNamespace},
    {"import",    LexicalKind::KeywordImport   },

    {"private",   LexicalKind::KeywordPrivate  },
    {"module",    LexicalKind::KeywordModule   },
    {"internal",  LexicalKind::KeywordInternal },
    {"protected", LexicalKind::KeywordProtected},
    {"public",    LexicalKind::KeywordPublic   },

    {"static",    LexicalKind::KeywordStatic   },
    {"mut",       LexicalKind::KeywordMutable  },
};

auto create_identifier_token(LexerContext& ctx) -> void {
  const auto str   = ctx.pop_buffer();
  const auto& kind = keywords.find(str);
  if (kind == keywords.end()) {
    ctx.tokens.emplace_back(ctx.token_position(), LexicalKind::Identifier, str);
  } else {
    create_empty_token(ctx, kind->second);
  }
}

/*
 * States
 */

auto identifier_end_exit_handler_(LexerContext& ctx) -> LexerState;
auto identifier_end_handler_(LexerContext& ctx) -> LexerState;
auto identifier_handler_(LexerContext& ctx) -> LexerState;
auto identifier_start_handler_(LexerContext& ctx) -> LexerState;

constexpr auto identifier_end_exit_state    = LexerState{identifier_end_exit_handler_};
constexpr auto identifier_end_state         = LexerState{identifier_end_handler_};
constexpr LexerState identifier_state       = LexerState{identifier_handler_};
constexpr LexerState identifier_start_state = LexerState{identifier_start_handler_};

auto identifier_end_exit_handler_(LexerContext& ctx) -> LexerState {
  create_identifier_token(ctx);
  return exit_state;
}

auto identifier_end_handler_(LexerContext& ctx) -> LexerState {
  create_identifier_token(ctx);
  return unknown_state;
}

auto identifier_handler_(LexerContext& ctx) -> LexerState {
#if LEXER_LOOP_OPTIMIZATION
  do {
    if (!matches_identifier(ctx.current())) {
      return identifier_end_state;
    }
    ctx.buffer_current();
  } while (ctx.move_next());
  return identifier_end_exit_state;
#else
  ctx.buffer_current();
  return ctx.move_next_state(
      matches_identifier, identifier_state, identifier_end_state, identifier_end_exit_state);
#endif
}

auto identifier_start_handler_(LexerContext& ctx) -> LexerState {
  ctx.mark_start_of_token();
  return identifier_state;
}