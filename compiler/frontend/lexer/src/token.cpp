// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "token.hpp"

/*
 * LexicalKind
 */

const auto lexical_kind_names_ = std::unordered_map<LexicalKind, std::string_view>{
    {LexicalKind::Unknown,                "Unknown"               },

    {LexicalKind::Identifier,             "Identifier"            },
    {LexicalKind::Number,                 "Number"                },
    {LexicalKind::String,                 "String"                },

    {LexicalKind::KeywordTrue,            "KeywordTrue"           },
    {LexicalKind::KeywordFalse,           "KeywordFalse"          },

    {LexicalKind::KeywordVar,             "KeywordVar"            },
    {LexicalKind::KeywordFunc,            "KeywordFunc"           },
    {LexicalKind::KeywordStruct,          "KeywordStruct"         },
    {LexicalKind::KeywordObject,          "KeywordObject"         },

    {LexicalKind::KeywordConcept,         "KeywordConcept"        },
    {LexicalKind::KeywordInterface,       "KeywordInterface"      },

    {LexicalKind::KeywordReturn,          "KeywordReturn"         },

    {LexicalKind::KeywordIf,              "KeywordIf"             },
    {LexicalKind::KeywordElif,            "KeywordElif"           },
    {LexicalKind::KeywordElse,            "KeywordElse"           },

    {LexicalKind::KeywordSwitch,          "KeywordSwitch"         },
    {LexicalKind::KeywordMatch,           "KeywordMatch"          },

    {LexicalKind::KeywordLoop,            "KeywordLoop"           },
    {LexicalKind::KeywordWhile,           "KeywordWhile"          },
    {LexicalKind::KeywordFor,             "KeywordFor"            },
    {LexicalKind::KeywordForeach,         "KeywordForeach"        },

    {LexicalKind::KeywordNamespace,       "KeywordNamespace"      },
    {LexicalKind::KeywordImport,          "KeywordImport"         },

    {LexicalKind::KeywordPrivate,         "KeywordPrivate"        },
    {LexicalKind::KeywordModule,          "KeywordModule"         },
    {LexicalKind::KeywordInternal,        "KeywordInternal"       },
    {LexicalKind::KeywordProtected,       "KeywordProtected"      },
    {LexicalKind::KeywordPublic,          "KeywordPublic"         },

    {LexicalKind::KeywordStatic,          "KeywordStatic"         },
    {LexicalKind::KeywordMutable,         "KeywordMutable"        },

    {LexicalKind::SymbolPeriod,           "SymbolPeriod"          },
    {LexicalKind::SymbolSemicolon,        "SymbolSemicolon"       },
    {LexicalKind::SymbolColon,            "SymbolColon"           },
    {LexicalKind::SymbolDoubleColon,      "SymbolDoubleColon"     },
    {LexicalKind::SymbolComma,            "SymbolComma"           },

    {LexicalKind::SymbolParenOpen,        "SymbolParenOpen"       },
    {LexicalKind::SymbolParenClose,       "SymbolParenClose"      },

    {LexicalKind::SymbolSquareOpen,       "SymbolSquareOpen"      },
    {LexicalKind::SymbolsquareClose,      "SymbolSquareClose"     },

    {LexicalKind::SymbolAngleOpen,        "SymbolAngleOpen"       },
    {LexicalKind::SymbolAngleClose,       "SymbolAngleClose"      },

    {LexicalKind::SymbolCurlyOpen,        "SymbolCurlyOpen"       },
    {LexicalKind::SymbolCurlyClose,       "SymbolCurlyClose"      },

    {LexicalKind::SymbolArrow,            "SymbolArrow"           },

    {LexicalKind::SymbolEquals,           "SymbolEquals"          },

    {LexicalKind::SymbolBoolEquals,       "SymbolBoolEquals"      },
    {LexicalKind::SymbolBoolNotEquals,    "SymbolBoolNotEquals"   },
    {LexicalKind::SymbolBoolNot,          "SymbolBoolNot"         },
    {LexicalKind::SymbolBoolOr,           "SymbolBoolOr"          },
    {LexicalKind::SymbolBoolAnd,          "SymbolBoolAnd"         },

    {LexicalKind::SymbolBitNot,           "SymbolBitNot"          },
    {LexicalKind::SymbolBitOr,            "SymbolBitOr"           },
    {LexicalKind::SymbolBitAnd,           "SymbolBitAnd"          },
    {LexicalKind::SymbolBitXor,           "SymbolBitXor"          },

    {LexicalKind::SymbolPlus,             "SymbolPlus"            },
    {LexicalKind::SymbolMinus,            "SymbolMinus"           },
    {LexicalKind::SymbolStar,             "SymbolStar"            },
    {LexicalKind::SymbolSlash,            "SymbolSlash"           },

    {LexicalKind::SymbolInc,              "SymbolInc"             },
    {LexicalKind::SymbolDec,              "SymbolDec"             },

    {LexicalKind::SymbolShiftLeft,        "SymbolShiftLeft"       },
    {LexicalKind::SymbolShiftRight,       "SymbolShiftRight"      },

    {LexicalKind::SymbolLessThanEqual,    "SymbolLessThanEqual"   },
    {LexicalKind::SymbolGreaterThanEqual, "SymbolGreaterThanEqual"},
};

auto to_string(LexicalKind kind) -> std::string_view {
  auto name = lexical_kind_names_.find(kind);
  if (name == lexical_kind_names_.end()) {
    throw std::exception("");
  }
  return name->second;
}

auto operator<<(std::ostream& stream, LexicalKind kind) -> std::ostream& {
  stream << to_string(kind);
  return stream;
}

/*
 * FilePosition
 */

FilePosition::FilePosition(FilePosition::int_type line, FilePosition::int_type col)
    : line_{line},
      col_{col} {}

auto FilePosition::to_string() const -> std::string {
  auto str = std::stringstream{};
  str << *this;
  return str.str();
}

auto operator<<(std::ostream& stream, const FilePosition& position) -> std::ostream& {
  stream << '(' << position.line() << ", " << position.col() << ')';
  return stream;
}

/*
 * LexicalToken
 */

constexpr auto token_node_name       = std::string_view{"Token"};

constexpr auto token_pos_attr_name   = std::string_view{"pos"};
constexpr auto token_kind_attr_name  = std::string_view{"kind"};
constexpr auto token_value_attr_name = std::string_view{"value"};

LexicalToken::LexicalToken(const FilePosition& pos, LexicalKind kind, ValueType value)
    : pos_{pos},
      kind_{kind},
      value_{std::move(value)} {}

auto& create_pos_attribute(xml::document& doc, const FilePosition& pos) {
  auto str      = pos.to_string();
  auto pos_str  = xml::allocate_string(doc, str);

  return xml::allocate_attribute(doc, token_pos_attr_name, pos_str);
}

auto& create_kind_attribute(xml::document& doc, LexicalKind kind) {
  return xml::allocate_attribute(doc, token_kind_attr_name, to_string(kind));
}

auto& create_value_attribute(xml::document& doc, const std::string& str) {
  return xml::allocate_attribute(doc, token_value_attr_name, str);
}

auto LexicalToken::xml_create_node(xml::document& doc) const -> xml::node& {
  auto& node = xml::allocate_element(doc, token_node_name);
  node.append_attribute(&create_pos_attribute(doc, pos()));
  node.append_attribute(&create_kind_attribute(doc, kind()));
  if (has_value()) {
    node.append_attribute(&create_value_attribute(doc, value()));
  }

  return node;
}

auto operator<<(std::ostream& stream, const LexicalToken& token) -> std::ostream& {
  auto doc = xml::document{};
  doc.append_node(&token.xml_create_node(doc));
  return stream << doc;
}

/*
 * TokenCollection
 */

constexpr auto token_collection_node_name = std::string_view{"TokenCollection"};
constexpr auto tokens_node_name           = std::string_view{"Tokens"};
constexpr auto token_source_attr_name     = std::string_view{"source"};

auto TokenCollection::xml_create_node(xml::document& doc) const -> xml::node& {
  auto& node        = xml::allocate_element(doc, token_collection_node_name);
  auto source_path = deref(source()).path().string();

  node.append_attribute(
      &xml::allocate_attribute(doc, token_source_attr_name, xml::allocate_string(doc, source_path)));

  auto& tokens_node = xml::allocate_element(doc, tokens_node_name);
  for (auto& token : tokens()) {
    tokens_node.append_node(&token.xml_create_node(doc));
  }
  node.append_node(&tokens_node);

  return node;
}

auto operator<<(std::ostream& stream, const TokenCollection& token_collection) -> std::ostream& {
  auto doc = rapidxml::xml_document<>{};
  doc.append_node(&token_collection.xml_create_node(doc));
  return stream << doc;
}
