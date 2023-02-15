
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "syntax_tree.hpp"

#include "xml/rapid_xml.hpp"

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
 * Xml Names
 */
constexpr auto base_syntax_node_name        = std::string_view{"Syntax"};

constexpr auto statement_node_name          = std::string_view{"Statement"};
constexpr auto block_node_name              = std::string_view{"StatementBlock"};
constexpr auto expr_stmt_node_name          = std::string_view{"ExpressionStatement"};
constexpr auto ret_stmt_node_name           = std::string_view{"ReturnStatement"};

constexpr auto expr_node_name               = std::string_view{"Expression"};
constexpr auto cnst_expr_node_name          = std::string_view{"ConstantExpression"};
constexpr auto bool_expr_node_name          = std::string_view{"Boolean"};
constexpr auto num_expr_node_name           = std::string_view{"Number"};
constexpr auto str_expr_node_name           = std::string_view{"String"};
constexpr auto ident_expr_node_name         = std::string_view{"Identifier"};

constexpr auto op_node_name                 = std::string_view{"Operation"};
constexpr auto unary_op_node_name           = std::string_view{"UnaryOperation"};
constexpr auto binary_op_node_name          = std::string_view{"BinaryOperation"};

constexpr auto body_stmt_node_name          = std::string_view{"BodyStatement"};
constexpr auto if_stmt_node_name            = std::string_view{"IfStatement"};
constexpr auto elif_stmt_node_name          = std::string_view{"ElifStatement"};
constexpr auto else_stmt_node_name          = std::string_view{"ElseStatement"};
constexpr auto loop_stmt_node_name          = std::string_view{"LoopStatement"};
constexpr auto while_stmt_node_name         = std::string_view{"WhileStatement"};
constexpr auto for_stmt_node_name           = std::string_view{"ForStatement"};
constexpr auto foreach_stmt_node_name       = std::string_view{"ForeachStatement"};

constexpr auto call_expr_node_name          = std::string_view{"Call"};

constexpr auto base_definition_node_name    = std::string_view{"Definition"};
constexpr auto var_def_node_name            = std::string_view{"Variable"};
constexpr auto func_def_node_name           = std::string_view{"Function"};
constexpr auto struct_def_node_name         = std::string_view{"Struct"};
constexpr auto object_def_node_name         = std::string_view{"Object"};

constexpr auto base_structure_def_node_name = std::string_view{"StructureDefinition"};

constexpr auto ns_node_name                 = std::string_view{"Namespace"};
constexpr auto import_node_name             = std::string_view{"Import"};

constexpr auto syntax_tree_node_name        = std::string_view{"SyntaxTree"};

constexpr auto assign_node_name             = std::string_view{"Assignment"};
constexpr auto params_node_name             = std::string_view{"Parameters"};
constexpr auto body_node_name               = std::string_view{"Body"};

constexpr auto pos_attr_name                = std::string_view{"pos"};
constexpr auto kind_attr_name               = std::string_view{"kind"};
constexpr auto name_attr_name               = std::string_view{"name"};
constexpr auto type_attr_name               = std::string_view{"type"};
constexpr auto value_attr_name              = std::string_view{"value"};
constexpr auto return_attr_name             = std::string_view{"return_type"};
constexpr auto op_attr_name                 = std::string_view{"operation"};

/*
 * BaseSyntax
 */

auto BaseSyntax::xml_node_name(xml::document& doc) const -> std::string_view {
  return base_syntax_node_name;
}

auto BaseSyntax::xml_append_elements(xml::document& doc, xml::node& node) const -> void {
  node.append_attribute(
      &xml::allocate_attribute(doc, pos_attr_name, xml::allocate_string(doc, pos().to_string())));
  // node.append_attribute(&xml::allocate_attribute(doc, kind_attr_name, to_string(kind())));
}

auto BaseSyntax::xml_create_node(xml::document& doc) const -> xml::node& {
  auto& node = xml::allocate_element(doc, xml_node_name(doc));
  this->xml_append_elements(doc, node);
  return node;
}

/*
 * BaseExpression
 */

std::string_view BaseExpression::xml_node_name(xml::document& doc) const { return expr_node_name; }

/*
 * BaseConstantExpression
 */

std::string_view BaseConstantExpression::xml_node_name(xml::document& doc) const {
  return cnst_expr_node_name;
}

/*
 * BooleanExpression
 */

std::string_view BooleanExpression::xml_node_name(xml::document& doc) const {
  return bool_expr_node_name;
}

void BooleanExpression::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);
  node.append_attribute(&xml::allocate_attribute(doc, value_attr_name, value() ? "true" : "false"));
}

/*
 * BooleanExpression
 */

std::string_view BaseConstantValueExpression::xml_node_name(xml::document& doc) const {
  return bool_expr_node_name;
}

void BaseConstantValueExpression::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);
}

/*
 * NumberExpression
 */

std::string_view NumberExpression::xml_node_name(xml::document& doc) const {
  return num_expr_node_name;
}

/*
 * StringExpression
 */

std::string_view StringExpression::xml_node_name(xml::document& doc) const {
  return str_expr_node_name;
}

/*
 * IdentifierExpression
 */

std::string_view IdentifierExpression::xml_node_name(xml::document& doc) const {
  return ident_expr_node_name;
}

void IdentifierExpression::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);
  doc.append_attribute(&xml::allocate_attribute(doc, value_attr_name, identifier()));
}

/*
 * CallExpression
 */

std::string_view CallExpression::xml_node_name(xml::document& doc) const {
  return call_expr_node_name;
}

void CallExpression::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);
}

/*
 * BaseOperation
 */

std::string_view BaseOperation::xml_node_name(xml::document& doc) const { return op_node_name; }

void BaseOperation::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);
  node.append_attribute(&xml::allocate_attribute(doc, op_attr_name, to_string(op())));
}

/*
 * UnaryExpression
 */

std::string_view UnaryExpression::xml_node_name(xml::document& doc) const {
  return unary_op_node_name;
}

void UnaryExpression::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseOperation::xml_append_elements(doc, node);
  node.append_node(&deref(expr()).xml_create_node(doc));
}

/*
 * BinaryExpression
 */

std::string_view BinaryExpression::xml_node_name(xml::document& doc) const {
  return binary_op_node_name;
}

void BinaryExpression::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseOperation::xml_append_elements(doc, node);
  node.append_node(&deref(lhs()).xml_create_node(doc));
  node.append_node(&deref(rhs()).xml_create_node(doc));
}

/*
 * BaseStatement
 */

std::string_view BaseStatement::xml_node_name(xml::document& doc) const {
  return statement_node_name;
}

/*
 * StatementBlock
 */

std::string_view StatementBlock::xml_node_name(xml::document& doc) const { return block_node_name; }

void StatementBlock::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);

  for (auto& statement : statements()) {
    node.append_node(&deref(statement).xml_create_node(doc));
  }
}

/*
 * ExpressionStatement
 */

std::string_view ExpressionStatement::xml_node_name(xml::document& doc) const {
  return expr_stmt_node_name;
}

void ExpressionStatement::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseStatement::xml_append_elements(doc, node);
  doc.append_node(&deref(expr()).xml_create_node(doc));
}

/*
 * ReturnStatement
 */

std::string_view ReturnStatement::xml_node_name(xml::document& doc) const {
  return ret_stmt_node_name;
}

void ReturnStatement::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseStatement::xml_append_elements(doc, node);
  node.append_node(&deref(expr()).xml_create_node(doc));
}

/*
 * BaseBodyStatement
 */

std::string_view BaseBodyStatement::xml_node_name(xml::document& doc) const {
  return body_stmt_node_name;
}

void BaseBodyStatement::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseStatement::xml_append_elements(doc, node);
  node.append_node(&deref(body()).xml_create_node(doc));
}

/*
 * IfStatement
 */

std::string_view IfStatement::xml_node_name(xml::document& doc) const { return if_stmt_node_name; }

void IfStatement::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseBodyStatement::xml_append_elements(doc, node);
  node.append_node(&deref(expr()).xml_create_node(doc));
}

/*
 * ElifStatement
 */

std::string_view ElifStatement::xml_node_name(xml::document& doc) const {
  return elif_stmt_node_name;
}

/*
 * ElseStatement
 */

std::string_view ElseStatement::xml_node_name(xml::document& doc) const {
  return else_stmt_node_name;
}

/*
 * LoopStatement
 */

std::string_view LoopStatement::xml_node_name(xml::document& doc) const {
  return loop_stmt_node_name;
}

/*
 * WhileStatement
 */

std::string_view WhileStatement::xml_node_name(xml::document& doc) const {
  return while_stmt_node_name;
}

void WhileStatement::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseBodyStatement::xml_append_elements(doc, node);
  node.append_node(&deref(expr()).xml_create_node(doc));
}

/*
 * ForStatement
 */

std::string_view ForStatement::xml_node_name(xml::document& doc) const {
  return for_stmt_node_name;
}

void ForStatement::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseBodyStatement::xml_append_elements(doc, node);
}

/*
 * BaseDefinition
 */

std::string_view BaseDefinition::xml_node_name(xml::document& doc) const {
  return base_definition_node_name;
}

void BaseDefinition::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);

  if (!name().empty()) {
    node.append_attribute(&xml::allocate_attribute(doc, name_attr_name, name()));
  }
}

/*
 * VariableDefinition
 */

std::string_view VariableDefinition::xml_node_name(xml::document& doc) const {
  return var_def_node_name;
}
void VariableDefinition::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseDefinition::xml_append_elements(doc, node);

  if (is_typed()) {
    node.append_attribute(&xml::allocate_attribute(doc, type_attr_name, type_name()));
  }

  if (is_assigned()) {
    auto& assign_node = xml::allocate_element(doc, assign_node_name);
    assign_node.append_node(&deref(assignment()).xml_create_node(doc));
  }
}

/*
 * FunctionDefinition
 */

std::string_view FunctionDefinition::xml_node_name(xml::document& doc) const {
  return func_def_node_name;
}
void FunctionDefinition::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseDefinition::xml_append_elements(doc, node);

  if (!is_return_auto()) {
    node.append_attribute(&xml::allocate_attribute(doc, return_attr_name, return_type()));
  }

  if (!parameters().empty()) {
    auto& params_node = xml::allocate_element(doc, params_node_name);
    for (auto& param : parameters()) {
      params_node.append_node(&deref(param).xml_create_node(doc));
    }
    node.append_node(&params_node);
  }

  if (body()) {
    // auto& body_node = xml::allocate_element(doc, body_node_name);
    node.append_node(&deref(body()).xml_create_node(doc));
    // node.append_node(&body_node);
  }
}

/*
 * BaseStructureDefinition
 */

std::string_view BaseStructureDefinition::xml_node_name(xml::document& doc) const {
  return base_structure_def_node_name;
}
void BaseStructureDefinition::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseDefinition::xml_append_elements(doc, node);

  for (auto& variable : variables()) {
    node.append_node(&deref(variable).xml_create_node(doc));
  }

  for (auto& function : functions()) {
    node.append_node(&deref(function).xml_create_node(doc));
  }

  for (auto& strct : structs()) {
    node.append_node(&deref(strct).xml_create_node(doc));
  }

  for (auto& object : objects()) {
    node.append_node(&deref(object).xml_create_node(doc));
  }
}

/*
 * StructDefinition
 */

std::string_view StructDefinition::xml_node_name(xml::document& doc) const {
  return struct_def_node_name;
}

/*
 * ObjectDefinition
 */

std::string_view ObjectDefinition::xml_node_name(xml::document& doc) const {
  return object_def_node_name;
}

/*
 * NamespaceDeclaration
 */

constexpr auto namespace_seperator = std::string_view{"::"};
auto create_full_namespace(const std::vector<std::string>& subs) -> std::string {
  auto name = std::string{};
  for (auto& ns : subs) {
    if (!name.empty()) {
      name.append(namespace_seperator);
    }
    name.append(ns);
  }
  return name;
}

auto NamespaceDeclaration::full_name() const -> std::string {
  return create_full_namespace(namespaces());
}

std::string_view NamespaceDeclaration::xml_node_name(xml::document& doc) const {
  return ns_node_name;
}

void NamespaceDeclaration::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);
  node.append_attribute(
      &xml::allocate_attribute(doc, name_attr_name, xml::allocate_string(doc, full_name())));
}

/*
 * NamespaceImport
 */

auto NamespaceImport::full_name() const -> std::string {
  return create_full_namespace(namespaces());
}

std::string_view NamespaceImport::xml_node_name(xml::document& doc) const {
  return import_node_name;
}

void NamespaceImport::xml_append_elements(xml::document& doc, xml::node& node) const {
  BaseSyntax::xml_append_elements(doc, node);
  node.append_attribute(
      &xml::allocate_attribute(doc, name_attr_name, xml::allocate_string(doc, full_name())));
}

/*
 * SyntaxTree
 */

const std::unique_ptr<NamespaceDeclaration> NamespaceDeclaration::root =
    std::make_unique<NamespaceDeclaration>(FilePosition{0, 0});

std::string_view SyntaxTree::xml_node_name(xml::document& doc) const {
  return syntax_tree_node_name;
}

void SyntaxTree::xml_append_elements(xml::document& doc, xml::node& node) const {
  for (auto& imprt : imports()) {
    node.append_node(&deref(imprt).xml_create_node(doc));
  }

  for (auto& ns : namespaces()) {
    node.append_node(&deref(ns).xml_create_node(doc));
  }

  BaseStructureDefinition::xml_append_elements(doc, node);
}

auto operator<<(std::ostream& stream, const SyntaxTree& tree) -> std::ostream& {
  auto doc = xml::document{};
  doc.append_node(&tree.xml_create_node(doc));
  return stream << doc;
}
