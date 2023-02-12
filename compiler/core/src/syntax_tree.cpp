
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "syntax_tree.hpp"

/*
 * Syntax Kind
 */

const auto syntax_kind_names = std::unordered_map<SyntaxKind, std::string_view>{
    {SyntaxKind::Source,         "Source"        },

    {SyntaxKind::Namespace,      "Namespace"     },
    {SyntaxKind::Import,         "Import"        },

    {SyntaxKind::ExprBool,       "ExprBool"      },
    {SyntaxKind::ExprNumber,     "ExprNumber"    },
    {SyntaxKind::ExprString,     "ExprString"    },
    {SyntaxKind::ExprIdentifier, "ExprIdentifier"},
    {SyntaxKind::ExprCall,       "ExprCall"      },

    {SyntaxKind::ExprUnary,      "ExprUnary"     },
    {SyntaxKind::ExprBinary,     "ExprBinary"    },
    {SyntaxKind::ExprTernary,    "ExprTernary"   },

    {SyntaxKind::StmtExpr,       "StmtExpr"      },
    {SyntaxKind::StmtDef,        "StmtDef"       },
    {SyntaxKind::StmtRet,        "StmtRet"       },
    {SyntaxKind::StmtIf,         "StmtIf"        },
    {SyntaxKind::StmtElif,       "StmtElif"      },
    {SyntaxKind::StmtElse,       "StmtElse"      },

    {SyntaxKind::StmtLoop,       "StmtLoop"      },
    {SyntaxKind::StmtWhile,      "StmtWhile"     },
    {SyntaxKind::StmtFor,        "StmtFor"       },
    {SyntaxKind::StmtForeach,    "StmtForeach"   },

    {SyntaxKind::Block,          "Block"         },

    {SyntaxKind::DefVar,         "DefVar"        },
    {SyntaxKind::DefFunc,        "DefFunc"       },
    {SyntaxKind::DefParam,       "DefParam"      },
    {SyntaxKind::DefStruct,      "DefStruct"     },
    {SyntaxKind::DefObject,      "DefObject"     },
    {SyntaxKind::DefInterface,   "DefInterface"  },
};

auto to_string(SyntaxKind kind) -> std::string_view {
  if (auto it = syntax_kind_names.find(kind); it != syntax_kind_names.end()) {
    return it->second;
  }
  throw std::exception();
}

/*
 * Operator
 */

const auto operator_names = std::unordered_map<Operator, std::string_view>{
    {Operator::Static,            "StaticAccess"     },
    {Operator::Access,            "Access"           },
    {Operator::Assign,            "Assign"           },

    {Operator::Add,               "Add"              },
    {Operator::Subtract,          "Subtract"         },
    {Operator::Multiply,          "Multiply"         },
    {Operator::Divide,            "Divide"           },

    {Operator::Equals,            "Equals"           },
    {Operator::NotEquals,         "NotEquals"        },
    {Operator::Or,                "Or"               },
    {Operator::And,               "And"              },

    {Operator::LessThan,          "LessThan"         },
    {Operator::GreaterThan,       "GreaterThan"      },
    {Operator::LessThanEquals,    "LessThanEquals"   },
    {Operator::GreaterThanEquals, "GreaterThanEquals"},

    {Operator::BitOr,             "BitOr"            },
    {Operator::BitXor,            "BitXor"           },
    {Operator::BitAnd,            "BitAnd"           },

    {Operator::ShiftLeft,         "ShiftLeft"        },
    {Operator::ShiftRight,        "ShiftRight"       },

    {Operator::BoolNot,           "BoolNot"          },
    {Operator::BitNot,            "BitNot"           },

    {Operator::Positive,          "Positive"         },
    {Operator::Negative,          "Negative"         },

    {Operator::PreInc,            "PreInc"           },
    {Operator::PreDec,            "PreDec"           },

    {Operator::PostInc,           "PostInc"          },
    {Operator::PostDec,           "PostDec"          }
};

auto to_string(Operator op) -> std::string_view {
  if (auto it = operator_names.find(op); it != operator_names.end()) {
    return it->second;
  }
  throw std::exception();
}

/*
 * Operator Maps
 */

const auto unary_pre_op_map = std::unordered_map<LexicalKind, Operator>{
    {LexicalKind::SymbolPlus,    Operator::Positive},
    {LexicalKind::SymbolMinus,   Operator::Negative},

    {LexicalKind::SymbolInc,     Operator::PreInc  },
    {LexicalKind::SymbolDec,     Operator::PreDec  },

    {LexicalKind::SymbolBoolNot, Operator::BoolNot },
    {LexicalKind::SymbolBitNot,  Operator::BitNot  },
};

const auto unary_post_op_map = std::unordered_map<LexicalKind, Operator>{
    {LexicalKind::SymbolInc, Operator::PostInc},
    {LexicalKind::SymbolDec, Operator::PostDec},
};

const auto binary_op_map = std::unordered_map<LexicalKind, Operator>{
    {LexicalKind::SymbolPeriod,           Operator::Access           },
    {LexicalKind::SymbolDoubleColon,      Operator::Static           },
    {LexicalKind::SymbolEquals,           Operator::Assign           },

    {LexicalKind::SymbolPlus,             Operator::Add              },
    {LexicalKind::SymbolMinus,            Operator::Subtract         },
    {LexicalKind::SymbolStar,             Operator::Multiply         },
    {LexicalKind::SymbolSlash,            Operator::Divide           },

    {LexicalKind::SymbolBoolEquals,       Operator::Equals           },
    {LexicalKind::SymbolBoolNotEquals,    Operator::NotEquals        },
    {LexicalKind::SymbolBoolOr,           Operator::Or               },
    {LexicalKind::SymbolBoolAnd,          Operator::And              },

    {LexicalKind::SymbolAngleOpen,        Operator::LessThan         },
    {LexicalKind::SymbolAngleClose,       Operator::GreaterThan      },
    {LexicalKind::SymbolLessThanEqual,    Operator::LessThanEquals   },
    {LexicalKind::SymbolGreaterThanEqual, Operator::GreaterThanEquals},

    {LexicalKind::SymbolBitOr,            Operator::BitOr            },
    {LexicalKind::SymbolBitAnd,           Operator::BitAnd           },
    {LexicalKind::SymbolBitXor,           Operator::BitXor           },

    {LexicalKind::SymbolShiftLeft,        Operator::ShiftLeft        },
    {LexicalKind::SymbolShiftRight,       Operator::ShiftRight       },
};

auto get_unary_pre_op(LexicalKind kind) -> Operator {
  auto it = unary_pre_op_map.find(kind);
  if (it == unary_pre_op_map.end()) {
    throw std::exception();
  }
  return it->second;
}

auto get_unary_post_op(LexicalKind kind) -> Operator {
  auto it = unary_post_op_map.find(kind);
  if (it == unary_post_op_map.end()) {
    throw std::exception();
  }
  return it->second;
}

auto get_binary_op(LexicalKind kind) -> Operator {
  auto it = binary_op_map.find(kind);
  if (it == binary_op_map.end()) {
    throw std::exception();
  }
  return it->second;
}

/*
 * Access Modifiers
 */

const auto access_modifier_names = std::unordered_map<AccessModifier, std::string_view>{
    {AccessModifier::Private,   "Private"  },
    {AccessModifier::Module,    "Module"   },
    {AccessModifier::Internal,  "Internal" },
    {AccessModifier::Protected, "Protected"},
    {AccessModifier::Public,    "Public"   },
};

auto to_string(AccessModifier modifier) -> std::string_view {
  auto name = access_modifier_names.find(modifier);
  if (name == access_modifier_names.end()) {
    throw std::exception();
  }
  return name->second;
}

/*
 * Syntax Tree
 */

const std::unique_ptr<Namespace> Namespace::root = std::make_unique<Namespace>();

auto SyntaxTree::get_namespace() -> Namespace* {
  auto ns = std::find_if(nodes_.begin(), nodes_.end(), [](auto& node) -> bool {
    return node->kind() == SyntaxKind::Namespace;
  });

  if (ns != nodes_.end()) {
    return ptr_cast<Namespace>(ns->get());
  }

  return nullptr;
}

/*
 * Xml Serialization Overrides
 */

#ifdef TRACE

auto SyntaxNode::on_serialize(xml::SerializationContext& context) const -> void {
  context.add_attribute("kind", to_string(kind()));
}

void BoolExpression::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  context.add_attribute("value", value_ ? "true" : "false");
}

void ValueExpression::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  if (!value_.empty()) {
    context.add_attribute("value", value_);
  }
}

void IdentifierExpression::on_serialize(xml::SerializationContext& context) const {
  ExpressionNode::on_serialize(context);
  if (!identifier_.empty()) {
    context.add_attribute("name", identifier_);
  }
}

auto CallExpression::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  if (!identifier_.empty()) {
    context.add_attribute("name", identifier_);
  }

  for (auto& param : parameters()) {
    context.add_element("param", *param);
  }
}

auto Operation::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  context.add_attribute("op", to_string(op()));
}

void UnaryExpression::on_serialize(xml::SerializationContext& context) const {
  Operation::on_serialize(context);
  if (expr_) {
    context.add_element("expr", *expr_);
  }
}

void BinaryExpression::on_serialize(xml::SerializationContext& context) const {
  Operation::on_serialize(context);
  if (lhs_) {
    context.add_element("lhs", *lhs_);
  }
  if (rhs_) {
    context.add_element("rhs", *rhs_);
  }
}

auto DefStatement::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  if (def_) {
    context.add_element("def", *def_);
  }
}

void ExprStatement::on_serialize(xml::SerializationContext& context) const {
  Statement::on_serialize(context);
  if (expr_) {
    context.add_element("expr", *expr_);
  }
}

void ReturnStatement::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  if (expr_) {
    context.add_element("expr", *expr_);
  }
}

void IfStatement::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  if (expr_) {
    context.add_element("pred", *expr_);
  }
  if (body()) {
    context.add_element("body", *body());
  }
}

auto WhileStatement::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  if (expr_) {
    context.add_element("pred", *expr_);
  }
  if (body()) {
    context.add_element("body", *body());
  }
}

auto ForStatement::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  if (prefix_) {
    context.add_element("pre", *prefix_);
  }
  if (cond_) {
    context.add_element("cond", *cond_);
  }
  if (postfix_) {
    context.add_element("post", *postfix_);
  }

  if (body()) {
    context.add_element("body", *body());
  }
}

void StatementBlock::on_serialize(xml::SerializationContext& context) const {
  //SyntaxNode::on_serialize(context);
  for (auto& statement : statements_) {
    context.add_element("statement", *statement);
  }
}

void Definition::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  if (!name_.empty()) {
    context.add_attribute("name", name_);
  }
  context.add_attribute("access", to_string(modifier_));
}

void VarDefinition::on_serialize(xml::SerializationContext& context) const {
  Definition::on_serialize(context);
  if (!type_name_.empty()) {
    context.add_attribute("type", type_name_);
  }

  if (assignment_) {
    context.add_element("assign", *assignment_);
  }
}

void FuncDefinition::on_serialize(xml::SerializationContext& context) const {
  Definition::on_serialize(context);
  if (!return_.empty()) {
    context.add_attribute("return", return_);
  }
  if (body_) {
    context.add_element("body", *body_);
  }
}

auto Namespace::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);

  auto str = std::string{};
  for (auto& ns : namespaces_) {
    str += "::";
    str += ns;
  }
  context.add_attribute("namespace", str);
}

auto Import::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);

  auto str = std::string{};
  for (auto& ns : namespaces_) {
    str += "::";
    str += ns;
  }
  context.add_attribute("namespace", str);
}

auto SyntaxTree::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  for (auto& node : nodes_) {
    context.add_element("node", *node);
  }
}

#endif
