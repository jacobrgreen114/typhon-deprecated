// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

// todo : come up with solution to differentiate shift operators from embedded templates

#include "lexer_symbol.hpp"

#include "lexer_comment.hpp"

#define is ==

constexpr auto period        = '.';
constexpr auto colon         = ':';
constexpr auto semicolon     = ';';

constexpr auto paren_open    = '(';
constexpr auto paren_close   = ')';

constexpr auto brace_open    = '[';
constexpr auto brace_close   = ']';

constexpr auto angle_open    = '<';
constexpr auto angle_close   = '>';

constexpr auto bracket_open  = '{';
constexpr auto bracket_close = '}';

constexpr auto equals        = '=';

constexpr auto pipe          = '|';
constexpr auto amp           = '&';
constexpr auto caret         = '^';

constexpr auto plus          = '+';
constexpr auto minus         = '-';
constexpr auto star          = '*';
constexpr auto slash         = '/';

constexpr auto is_period(char c) -> bool { return c is period; }
constexpr auto is_colon(char c) -> bool { return c is colon; }
constexpr auto is_semicolon(char c) -> bool { return c is semicolon; }

constexpr auto is_paren_open(char c) -> bool { return c is paren_open; }
constexpr auto is_paren_close(char c) -> bool { return c is paren_close; }

constexpr auto is_brace_open(char c) -> bool { return c is brace_open; }
constexpr auto is_brace_close(char c) -> bool { return c is brace_close; }

constexpr auto is_angle_open(char c) -> bool { return c is angle_open; }
constexpr auto is_angle_close(char c) -> bool { return c is angle_close; }

constexpr auto is_bracket_open(char c) -> bool { return c is bracket_open; }
constexpr auto is_bracket_close(char c) -> bool { return c is bracket_close; }

constexpr auto is_equals(char c) -> bool { return c is equals; }

constexpr auto is_pipe(char c) -> bool { return c is pipe; }
constexpr auto is_amp(char c) -> bool { return c is amp; }
constexpr auto is_caret(char c) -> bool { return c is caret; }

constexpr auto is_plus(char c) -> bool { return c is plus; }
constexpr auto is_minus(char c) -> bool { return c is minus; }
constexpr auto is_star(char c) -> bool { return c is star; }
constexpr auto is_slash(char c) -> bool { return c is slash; }

#undef is

constexpr auto create_symbol_token(LexerContext& ctx, LexicalKind kind) -> void {
  create_empty_token(ctx, kind);
}

constexpr auto symbol_error_state              = LexerState{[](LexerContext& ctx) -> LexerState {
  std::cout << "Symbol error at " << ctx.token_position() << std::endl;
  throw_not_implemented();
}};

// Symbol Colon States

constexpr auto symbol_period_state             = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_period(current));

  create_symbol_token(ctx, LexicalKind::SymbolPeriod);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// Symbol Colon States

constexpr auto symbol_colon_state              = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_colon(current));

  create_symbol_token(ctx, LexicalKind::SymbolColon);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// Symbol Semicolon States

constexpr auto symbol_semicolon_state          = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_semicolon(current));

  create_symbol_token(ctx, LexicalKind::SymbolSemicolon);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// Symbol Brace States

constexpr auto symbol_paren_open_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_paren_open(current));

  create_symbol_token(ctx, LexicalKind::SymbolParenOpen);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_paren_close_state        = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_paren_close(current));

  create_symbol_token(ctx, LexicalKind::SymbolParenClose);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// Symbol Brace States

constexpr auto symbol_brace_open_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_brace_open(current));

  create_symbol_token(ctx, LexicalKind::SymbolBraceOpen);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_brace_close_state        = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_brace_close(current));

  create_symbol_token(ctx, LexicalKind::SymbolBraceClose);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// Symbol Angle Open States

// <
constexpr auto symbol_angle_open_end_state     = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolAngleOpen);
  return exit_state;
}};

// <
constexpr auto symbol_angle_open_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolAngleOpen);
  return unknown_state;
}};

// <<
constexpr auto symbol_shift_left_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_angle_open(current));

  create_symbol_token(ctx, LexicalKind::SymbolShiftLeft);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// <=
constexpr auto symbol_angle_open_equal_state   = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolLessThanEqual);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_angle_open_unknown_state = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_angle_open(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 1>{
  // LexerMatchCondition{is_angle_open, symbol_shift_left_state},
      LexerMatchCondition{is_equals, symbol_angle_open_equal_state}
  };

  return ctx.move_next_state(symbol_angle_open_state, symbol_angle_open_end_state, conditions);
}};

#pragma region Symbol Angle Close States

// >
constexpr auto symbol_angle_close_end_state     = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolAngleClose);
  return exit_state;
}};

// >
constexpr auto symbol_angle_close_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolAngleClose);
  return unknown_state;
}};

// >>
constexpr auto symbol_shift_right_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_angle_close(current));

  create_symbol_token(ctx, LexicalKind::SymbolShiftRight);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// >=
constexpr auto symbol_angle_close_equal_state   = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolGreaterThanEqual);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_angle_close_unknown_state = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_plus(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 1>{
  // LexerMatchCondition{is_angle_close, symbol_shift_right_state},
      LexerMatchCondition{is_equals, symbol_angle_close_equal_state}
  };

  return ctx.move_next_state(symbol_angle_close_state, symbol_angle_close_end_state, conditions);
}};

// Symbol Plus States

// +
constexpr auto symbol_plus_end_state            = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolPlus);
  return exit_state;
}};

// +
constexpr auto symbol_plus_state                = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolPlus);
  return unknown_state;
}};

// ++
constexpr auto symbol_plus_plus_state           = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_plus(current));

  create_symbol_token(ctx, LexicalKind::SymbolInc);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// +=
constexpr auto symbol_plus_equal_state          = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolPlusEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_plus_unknown_state        = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_plus(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 2>{
      LexerMatchCondition{is_plus,   symbol_plus_plus_state },
      LexerMatchCondition{is_equals, symbol_plus_equal_state}
  };

  return ctx.move_next_state(symbol_plus_state, symbol_plus_end_state, conditions);
}};

// Symbol Bracket States

constexpr auto symbol_bracket_open_state        = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_bracket_open(current));

  create_symbol_token(ctx, LexicalKind::SymbolBracketOpen);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_bracket_close_state       = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_bracket_close(current));

  create_symbol_token(ctx, LexicalKind::SymbolBracketClose);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// Symbol Equals States

// =
constexpr auto symbol_equals_end_state          = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolEquals);
  return exit_state;
}};

// =
constexpr auto symbol_equals_state              = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolEquals);
  return unknown_state;
}};

// ==
constexpr auto symbol_equals_equal_state        = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolBoolEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_equals_unknown_state      = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 1>{
      LexerMatchCondition{is_equals, symbol_equals_equal_state}
  };

  return ctx.move_next_state(symbol_equals_state, symbol_equals_end_state, conditions);
}};

// Symbol Pipe States

// |
constexpr auto symbol_pipe_end_state            = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolBitOr);
  return exit_state;
}};

// |
constexpr auto symbol_pipe_state                = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolBitOr);
  return unknown_state;
}};

// ||
constexpr auto symbol_pipe_pipe_state           = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_pipe(current));

  create_symbol_token(ctx, LexicalKind::SymbolBoolOr);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// |=
constexpr auto symbol_pipe_equal_state          = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolBitOrEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_pipe_unknown_state        = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_pipe(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 2>{
      LexerMatchCondition{is_pipe,   symbol_pipe_pipe_state },
      LexerMatchCondition{is_equals, symbol_pipe_equal_state}
  };

  return ctx.move_next_state(symbol_pipe_state, symbol_pipe_end_state, conditions);
}};

// Symbol Amp States

// &
constexpr auto symbol_amp_end_state             = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolBitAnd);
  return exit_state;
}};

// &
constexpr auto symbol_amp_state                 = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolBitAnd);
  return unknown_state;
}};

// &&
constexpr auto symbol_amp_amp_state             = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_amp(current));

  create_symbol_token(ctx, LexicalKind::SymbolBoolAnd);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// &=
constexpr auto symbol_amp_equal_state           = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolBitAndEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_amp_unknown_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_amp(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 2>{
      LexerMatchCondition{is_amp,    symbol_amp_amp_state  },
      LexerMatchCondition{is_equals, symbol_amp_equal_state}
  };

  return ctx.move_next_state(symbol_amp_state, symbol_amp_end_state, conditions);
}};

// Symbol Amp States

// ^
constexpr auto symbol_caret_end_state           = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolBitXor);
  return exit_state;
}};

// ^
constexpr auto symbol_caret_state               = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolBitXor);
  return unknown_state;
}};

// ^=
constexpr auto symbol_caret_equal_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolBitXorEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_caret_unknown_state       = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_caret(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 1>{
      LexerMatchCondition{is_equals, symbol_caret_equal_state}
  };

  return ctx.move_next_state(symbol_caret_state, symbol_caret_end_state, conditions);
}};

// Symbol Minus States

// -
constexpr auto symbol_minus_end_state           = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolMinus);
  return exit_state;
}};

// -
constexpr auto symbol_minus_state               = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolMinus);
  return unknown_state;
}};

// --
constexpr auto symbol_minus_minus_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_minus(current));

  create_symbol_token(ctx, LexicalKind::SymbolDec);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// -=
constexpr auto symbol_minus_equal_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolMinusEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

// ->
constexpr auto symbol_arrow_state               = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_angle_close(current));

  create_symbol_token(ctx, LexicalKind::SymbolArrow);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_minus_unknown_state       = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_minus(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 3>{
      LexerMatchCondition{is_minus,       symbol_minus_minus_state},
      LexerMatchCondition{is_equals,      symbol_minus_equal_state},
      LexerMatchCondition{is_angle_close, symbol_arrow_state      }
  };

  return ctx.move_next_state(symbol_minus_state, symbol_minus_end_state, conditions);
}};

// Symbol Star States

// *
constexpr auto symbol_star_end_state            = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolStar);
  return exit_state;
}};

// *
constexpr auto symbol_star_state                = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolStar);
  return unknown_state;
}};

// *=
constexpr auto symbol_star_equal_state          = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolStarEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_star_unknown_state        = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_star(current));

  constexpr auto conditions = std::array<LexerMatchCondition, 1>{
      LexerMatchCondition{is_equals, symbol_star_equal_state}
  };

  return ctx.move_next_state(symbol_star_state, symbol_star_end_state, conditions);
}};

// Symbol Slash States

// /
constexpr auto symbol_slash_end_state           = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolSlash);
  return exit_state;
}};

// /
constexpr auto symbol_slash_state               = LexerState{[](LexerContext& ctx) -> LexerState {
  create_symbol_token(ctx, LexicalKind::SymbolSlash);
  return unknown_state;
}};

// /=
constexpr auto symbol_slash_equal_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_equals(current));

  create_symbol_token(ctx, LexicalKind::SymbolSlashEquals);
  return ctx.move_next_state(unknown_state, exit_state);
}};

constexpr auto symbol_slash_unknown_state       = LexerState{[](LexerContext& ctx) -> LexerState {
  auto current = ctx.current();
  assert(is_slash(current));

  const auto conditions = std::array<LexerMatchCondition, 3>{
      LexerMatchCondition{is_slash,  comment_singleline_start_state},
      LexerMatchCondition{is_star,   comment_multiline_start_state },
      LexerMatchCondition{is_equals, symbol_slash_equal_state      }
  };

  return ctx.move_next_state(symbol_slash_state, symbol_slash_end_state, conditions);
}};

// Symbol Start States

constexpr LexerState symbol_unknown_state       = LexerState{[](LexerContext& ctx) -> LexerState {
  auto& current = ctx.current();
  switch (current) {
    case period:
      return symbol_period_state;

    case colon:
      return symbol_colon_state;
    case semicolon:
      return symbol_semicolon_state;

    case paren_open:
      return symbol_paren_open_state;
    case paren_close:
      return symbol_paren_close_state;

    case brace_open:
      return symbol_brace_open_state;
    case brace_close:
      return symbol_brace_close_state;

    case angle_open:
      return symbol_angle_open_unknown_state;

    case bracket_open:
      return symbol_bracket_open_state;
    case bracket_close:
      return symbol_bracket_close_state;

    case equals:
      return symbol_equals_unknown_state;

    case pipe:
      return symbol_pipe_unknown_state;
    case amp:
      return symbol_amp_unknown_state;
    case caret:
      return symbol_caret_unknown_state;

    case plus:
      return symbol_plus_unknown_state;
    case minus:
      return symbol_minus_unknown_state;
    case star:
      return symbol_star_unknown_state;
    case slash:
      return symbol_slash_unknown_state;
    default:
      return symbol_error_state;
  }
}};

constexpr LexerState symbol_start_state         = LexerState{[](LexerContext& ctx) -> LexerState {
  ctx.mark_start_of_token();
  return symbol_unknown_state;
}};