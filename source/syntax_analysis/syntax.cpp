
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "syntax.hpp"

/*
 * Syntax Kind
 */

const auto syntax_kind_names =
    std::unordered_map<SyntaxKind, std::string_view>{ENUM_NAME(SyntaxKind, Source),

                                                     ENUM_NAME(SyntaxKind, ExprNumber),
                                                     ENUM_NAME(SyntaxKind, ExprString),
                                                     ENUM_NAME(SyntaxKind, ExprIdentifier),

                                                     ENUM_NAME(SyntaxKind, ExprUnary),
                                                     ENUM_NAME(SyntaxKind, ExprBinary),
                                                     ENUM_NAME(SyntaxKind, ExprTernary),

                                                     ENUM_NAME(SyntaxKind, StmExpr),
                                                     ENUM_NAME(SyntaxKind, StmReturn),
                                                     ENUM_NAME(SyntaxKind, StmIf),
                                                     ENUM_NAME(SyntaxKind, StmElif),
                                                     ENUM_NAME(SyntaxKind, StmElse),

                                                     ENUM_NAME(SyntaxKind, Block),

                                                     ENUM_NAME(SyntaxKind, DefVar),
                                                     ENUM_NAME(SyntaxKind, DefFunc),
                                                     ENUM_NAME(SyntaxKind, DefParam),
                                                     ENUM_NAME(SyntaxKind, DefStruct),
                                                     ENUM_NAME(SyntaxKind, DefClass),
                                                     ENUM_NAME(SyntaxKind, DefInterface)};

auto to_string(SyntaxKind kind) -> const std::string_view& {
  if (auto it = syntax_kind_names.find(kind); it != syntax_kind_names.end()) {
    return it->second;
  }
  throw std::exception();
}

/*
 * Operator
 */

const auto operator_names =
    std::unordered_map<Operator, std::string_view>{ENUM_NAME(Operator, Access),

                                                   ENUM_NAME(Operator, Add),
                                                   ENUM_NAME(Operator, Subtract),
                                                   ENUM_NAME(Operator, Multiply),
                                                   ENUM_NAME(Operator, Divide),

                                                   ENUM_NAME(Operator, Equals),
                                                   ENUM_NAME(Operator, NotEquals),
                                                   ENUM_NAME(Operator, Or),
                                                   ENUM_NAME(Operator, And),

                                                   ENUM_NAME(Operator, LessThan),
                                                   ENUM_NAME(Operator, GreaterThan),
                                                   ENUM_NAME(Operator, LessThanEquals),
                                                   ENUM_NAME(Operator, GreaterThanEquals),

                                                   ENUM_NAME(Operator, BitOr),
                                                   ENUM_NAME(Operator, BitXor),
                                                   ENUM_NAME(Operator, BitAnd),

                                                   ENUM_NAME(Operator, ShiftLeft),
                                                   ENUM_NAME(Operator, ShiftRight),

                                                   ENUM_NAME(Operator, BoolNot),
                                                   ENUM_NAME(Operator, BitNot),

                                                   ENUM_NAME(Operator, Positive),
                                                   ENUM_NAME(Operator, Negative),

                                                   ENUM_NAME(Operator, PreInc),
                                                   ENUM_NAME(Operator, PreDec),

                                                   ENUM_NAME(Operator, PostInc),
                                                   ENUM_NAME(Operator, PostDec)};

auto to_string(Operator op) -> const std::string_view& {
  if (auto it = operator_names.find(op); it != operator_names.end()) {
    return it->second;
  }
  throw std::exception();
}

const auto expressions =
    std::unordered_set<SyntaxKind>{SyntaxKind::ExprNumber, SyntaxKind::ExprBinary};

const auto statements = std::unordered_set<SyntaxKind>{
    SyntaxKind::StmExpr,
    SyntaxKind::StmReturn,
    SyntaxKind::StmIf,
    SyntaxKind::StmElif,
    SyntaxKind::StmElse,
};

const auto definitions = std::unordered_set<SyntaxKind>{
    SyntaxKind::DefVar,
    SyntaxKind::DefFunc,
    SyntaxKind::DefParam,
    SyntaxKind::DefStruct,
    SyntaxKind::DefClass,
    SyntaxKind::DefInterface,
};

const auto unary_pre_op_map = std::unordered_map<LexicalKind, Operator>{
    {LexicalKind::SymbolPlus, Operator::Positive},
    {LexicalKind::SymbolMinus, Operator::Negative},

    {LexicalKind::SymbolInc, Operator::PreInc},
    {LexicalKind::SymbolDec, Operator::PreDec},

    {LexicalKind::SymbolBoolNot, Operator::BoolNot},
    {LexicalKind::SymbolBitNot, Operator::BitNot},
};

const auto unary_post_op_map = std::unordered_map<LexicalKind, Operator>{
    {LexicalKind::SymbolInc, Operator::PostInc},
    {LexicalKind::SymbolDec, Operator::PostDec},
};

const auto binary_op_map = std::unordered_map<LexicalKind, Operator>{
    {LexicalKind::SymbolPeriod, Operator::Access},

    {LexicalKind::SymbolPlus, Operator::Add},
    {LexicalKind::SymbolMinus, Operator::Subtract},
    {LexicalKind::SymbolStar, Operator::Multiply},
    {LexicalKind::SymbolSlash, Operator::Divide},

    {LexicalKind::SymbolBoolEquals, Operator::Equals},
    {LexicalKind::SymbolBoolNotEquals, Operator::NotEquals},
    {LexicalKind::SymbolBoolOr, Operator::Or},
    {LexicalKind::SymbolBoolAnd, Operator::And},

    {LexicalKind::SymbolAngleOpen, Operator::LessThan},
    {LexicalKind::SymbolAngleClose, Operator::GreaterThan},
    {LexicalKind::SymbolLessThanEqual, Operator::LessThanEquals},
    {LexicalKind::SymbolGreaterThanEqual, Operator::GreaterThanEquals},

    {LexicalKind::SymbolBitOr, Operator::BitOr},
    {LexicalKind::SymbolBitAnd, Operator::BitAnd},
    {LexicalKind::SymbolBitXor, Operator::BitXor},

    {LexicalKind::SymbolShiftLeft, Operator::ShiftLeft},
    {LexicalKind::SymbolShiftRight, Operator::ShiftRight},
};

const auto operator_precedence_map = std::unordered_map<Operator, Precedence>{
    {Operator::Access, Precedence::Access},

    {Operator::BitOr, Precedence::BitOr},
    {Operator::BitXor, Precedence::BitXor},
    {Operator::BitAnd, Precedence::BitAnd},

    {Operator::Equals, Precedence::Equality},
    {Operator::NotEquals, Precedence::Equality},

    {Operator::LessThan, Precedence::Relation},
    {Operator::GreaterThan, Precedence::Relation},
    {Operator::LessThanEquals, Precedence::Relation},
    {Operator::GreaterThanEquals, Precedence::Relation},

    {Operator::ShiftLeft, Precedence::Shift},
    {Operator::ShiftRight, Precedence::Shift},

    {Operator::Add, Precedence::AddSub},
    {Operator::Subtract, Precedence::AddSub},

    {Operator::Multiply, Precedence::MulDiv},
    {Operator::Divide, Precedence::MulDiv},

    {Operator::BoolNot, Precedence::Prefix},
    {Operator::BitNot, Precedence::Prefix},
    {Operator::Positive, Precedence::Prefix},
    {Operator::Negative, Precedence::Prefix},
    {Operator::PreInc, Precedence::Prefix},
    {Operator::PreDec, Precedence::Prefix},

    {Operator::PostInc, Precedence::Postfix},
    {Operator::PostDec, Precedence::Postfix},
};

// bool is_expression(SyntaxKind kind) { return expressions.contains(kind); }

// auto is_statement(SyntaxKind kind) -> bool { return statements.contains(kind); }

// auto is_definition(SyntaxKind kind) -> bool { return definitions.contains(kind); }

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

// auto get_precedence(Operator op) -> Precedence {
//   auto it = operator_precedence_map.find(op);
//   if (it == operator_precedence_map.end()) {
//     throw std::exception();
//   }
//   return it->second;
// }

#ifdef TRACE

auto SyntaxNode::on_serialize(xml::SerializationContext& context) const -> void {
  context.add_attribute("kind", to_string(kind()));
}

void ConstantExpression::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  if (value_) {
    context.add_attribute("value", *value_);
  }
}

void IdentifierExpression::on_serialize(xml::SerializationContext& context) const {
  ExpressionNode::on_serialize(context);
  if (identifier_) {
    context.add_attribute("name", *identifier_);
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

void StatementBlock::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  for (auto& statement : statements_) {
    context.add_element("statement", *statement);
  }
}

void Definition::on_serialize(xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  if (name_) {
    context.add_attribute("name", name_->c_str());
  }
}

void VarDefinition::on_serialize(xml::SerializationContext& context) const {
  Definition::on_serialize(context);
  if (type_name_) {
    context.add_attribute("type", type_name_->c_str());
  }

  if (assignment_) {
    context.add_element("assign", *assignment_);
  }
}

void FuncDefinition::on_serialize(xml::SerializationContext& context) const {
  Definition::on_serialize(context);
  if (return_) {
    context.add_attribute("return", *return_);
  }
  if (body_) {
    context.add_element("body", *body_);
  }
}

auto SourceNode::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  for (auto& node : nodes_) {
    context.add_element("node", *node);
  }
}

#endif
