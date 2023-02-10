// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_common.hpp"

const auto c_type_map = std::unordered_map<std::string_view, std::string_view>{
    {"bool32", "uint8_t" },

    {"u8",     "uint8_t" },
    {"u16",    "uint16_t"},
    {"u32",    "uint32_t"},
    {"u64",    "uint64_t"},

    {"i8",     "int8_t"  },
    {"i16",    "int16_t" },
    {"i32",    "int32_t" },
    {"i64",    "int64_t" },

    {"flt",    "float"   },
    {"dbl",    "double"  }
};

auto mutate_type_name(std::string_view view) -> std::string_view {
  if (auto c_type = c_type_map.find(view); c_type != c_type_map.end()) {
    return c_type->second;
  }
  return view;
}

const auto operator_symbol_map = std::unordered_map<Operator, std::string_view>{
    {Operator::Static,            "::"},
    {Operator::Access,            "." },
    {Operator::Assign,            "=" },

    {Operator::Add,               "+" },
    {Operator::Subtract,          "-" },
    {Operator::Multiply,          "*" },
    {Operator::Divide,            "/" },

    {Operator::Equals,            "=="},
    {Operator::NotEquals,         "!="},
    {Operator::Or,                "||"},
    {Operator::And,               "&&"},

    {Operator::LessThan,          "<" },
    {Operator::GreaterThan,       ">" },
    {Operator::LessThanEquals,    "<="},
    {Operator::GreaterThanEquals, ">="},

    {Operator::BitOr,             "|" },
    {Operator::BitXor,            "^" },
    {Operator::BitAnd,            "&" },

    {Operator::ShiftLeft,         "<<"},
    {Operator::ShiftRight,        ">>"},

    {Operator::BoolNot,           "!" },
    {Operator::BitNot,            "~" },

    {Operator::Positive,          "+" },
    {Operator::Negative,          "-" },

    {Operator::PreInc,            "++"},
    {Operator::PreDec,            "--"},

    {Operator::PostInc,           "++"},
    {Operator::PostDec,           "--"},
};

auto get_operator_symbol(Operator op) -> std::string_view {
  const auto sym = operator_symbol_map.find(op);
  if (sym == operator_symbol_map.end()) {
    throw std::exception();
  }
  return sym->second;
}