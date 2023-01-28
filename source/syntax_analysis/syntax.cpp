
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

bool is_expression(SyntaxKind kind) {
  switch (kind) {
    case SyntaxKind::ExprNumber:
    case SyntaxKind::ExprBinary: {
      return true;
    }
    default:
      return false;
  }
}

auto is_statement(SyntaxKind kind) -> bool {
  switch (kind) {
    case SyntaxKind::StmExpr:
    case SyntaxKind::StmReturn:
      return true;
    default:
      return false;
  }
}

auto is_definition(SyntaxKind kind) -> bool {
  switch (kind) {
    case SyntaxKind::DefVar:
    case SyntaxKind::DefFunc:
    case SyntaxKind::DefParam:
    case SyntaxKind::DefStruct:
    case SyntaxKind::DefClass:
    case SyntaxKind::DefInterface:
      return true;

    default:
      return false;
  }
}

auto get_unary_pre_op(LexicalKind kind) -> Operator {
  switch (kind) {
    case LexicalKind::SymbolPlus:
      return Operator::Positive;
    case LexicalKind::SymbolMinus:
      return Operator::Negative;
    case LexicalKind::SymbolInc:
      return Operator::PreInc;
    case LexicalKind::SymbolDec:
      return Operator::PreDec;

    case LexicalKind::SymbolBoolNot:
      return Operator::BoolNot;
    case LexicalKind::SymbolBitNot:
      return Operator::BitNot;
    default:
      throw std::exception();
  }
}

auto get_unary_post_op(LexicalKind kind) -> Operator {
  switch (kind) {
    case LexicalKind::SymbolInc:
      return Operator::PostInc;
    case LexicalKind::SymbolDec:
      return Operator::PostDec;
    default:
      throw std::exception();
  }
}

auto get_binary_op(LexicalKind kind) -> Operator {
  switch (kind) {
    case LexicalKind::SymbolPeriod:
      return Operator::Access;

    case LexicalKind::SymbolPlus:
      return Operator::Add;
    case LexicalKind::SymbolMinus:
      return Operator::Subtract;
    case LexicalKind::SymbolStar:
      return Operator::Multiply;
    case LexicalKind::SymbolSlash:
      return Operator::Divide;

    case LexicalKind::SymbolBoolEquals:
      return Operator::Equals;
    case LexicalKind::SymbolBoolNotEquals:
      return Operator::NotEquals;
    case LexicalKind::SymbolBoolOr:
      return Operator::Or;
    case LexicalKind::SymbolBoolAnd:
      return Operator::And;

    case LexicalKind::SymbolAngleOpen:
      return Operator::LessThan;
    case LexicalKind::SymbolAngleClose:
      return Operator::GreaterThan;
    case LexicalKind::SymbolLessThanEqual:
      return Operator::LessThanEquals;
    case LexicalKind::SymbolGreaterThanEqual:
      return Operator::GreaterThanEquals;

    case LexicalKind::SymbolBitOr:
      return Operator::BitOr;
    case LexicalKind::SymbolBitAnd:
      return Operator::BitAnd;
    case LexicalKind::SymbolBitXor:
      return Operator::BitXor;

    case LexicalKind::SymbolShiftLeft:
      return Operator::ShiftLeft;
    case LexicalKind::SymbolShiftRight:
      return Operator::ShiftRight;

    default:
      throw std::exception();
  }
}

auto get_precedence(Operator op) -> Precedence {
  switch (op) {
    case Operator::Access:
      return Precedence::Access;

    case Operator::BitOr:
      return Precedence::BitOr;
    case Operator::BitXor:
      return Precedence::BitXor;
    case Operator::BitAnd:
      return Precedence::BitAnd;

    case Operator::Equals:
    case Operator::NotEquals:
      return Precedence::Equality;

    case Operator::LessThan:
    case Operator::GreaterThan:
    case Operator::LessThanEquals:
    case Operator::GreaterThanEquals:
      return Precedence::Relation;

    case Operator::ShiftLeft:
    case Operator::ShiftRight:
      return Precedence::Shift;

    case Operator::Add:
    case Operator::Subtract:
      return Precedence::AddSub;

    case Operator::Multiply:
    case Operator::Divide:
      return Precedence::MulDiv;

    case Operator::BoolNot:
    case Operator::BitNot:
    case Operator::Positive:
    case Operator::Negative:
    case Operator::PreInc:
    case Operator::PreDec:
      return Precedence::Prefix;

    case Operator::PostInc:
    case Operator::PostDec:
      return Precedence::Postfix;

    default:
      throw std::exception();
  }
}

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
