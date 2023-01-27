
// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "syntax.hpp"

auto to_string(SyntaxKind kind) -> const char* {
  switch (kind) {
    case SyntaxKind::Source:
      return "Source";

    case SyntaxKind::ExprNumber:
      return "ExprNumber";
    case SyntaxKind::ExprUnary:
      return "ExprUnary";
    case SyntaxKind::ExprBinary:
      return "ExprBinary";

    case SyntaxKind::Block:
      return "Block";
    case SyntaxKind::StmExpr:
      return "StmExpr";
    case SyntaxKind::StmReturn:
      return "StmReturn";

    case SyntaxKind::DefVar:
      return "DefVar";
    case SyntaxKind::DefFunc:
      return "DefFunc";
    case SyntaxKind::DefParam:
      return "DefParam";
    case SyntaxKind::DefStruct:
      return "DefStruct";
    case SyntaxKind::DefClass:
      return "DefClass";
    case SyntaxKind::DefInterface:
      return "DefInterface";

    default:
      throw_not_implemented();
  }
}

auto to_string(Operator op) -> const char* {
  switch (op) {
    case Operator::Add:
      return "Add";
    case Operator::Subtract:
      return "Subtract";
    case Operator::Multiply:
      return "Multiply";
    case Operator::Divide:
      return "Divide";

    case Operator::Equals:
      return "Equals";
    case Operator::NotEquals:
      return "NotEquals";
    case Operator::Or:
      return "Or";
    case Operator::And:
      return "And";

    case Operator::LessThan:
      return "LessThan";
    case Operator::GreaterThan:
      return "GreaterThan";
    case Operator::LessThanEquals:
      return "LessThanEquals";
    case Operator::GreaterThanEquals:
      return "GreaterThanEquals";

    case Operator::BitOr:
      return "BitOr";
    case Operator::BitAnd:
      return "BitAnd";
    case Operator::BitXor:
      return "BitXor";

    case Operator::ShiftLeft:
      return "ShiftLeft";
    case Operator::ShiftRight:
      return "ShiftRight";

    case Operator::BoolNot:
      return "BoolNot";
    case Operator::BitNot:
      return "BitNot";

    case Operator::Positive:
      return "Positive";
    case Operator::Negative:
      return "Negative";

    case Operator::PreInc:
      return "PreInc";
    case Operator::PreDec:
      return "PreDec";

    case Operator::PostInc:
      return "PostInc";
    case Operator::PostDec:
      return "PostDec";

    default:
      throw_not_implemented();
  }
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

auto SyntaxNode::on_serialize(xml::SerializationContext& context) const
    -> void {
  context.add_attribute("kind", to_string(kind()));
}

void ConstantExpressionNode::on_serialize(
    xml::SerializationContext& context) const {
  SyntaxNode::on_serialize(context);
  if (value_) {
    context.add_attribute("value", *value_);
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

auto SourceNode::on_serialize(xml::SerializationContext& context) const
    -> void {
  SyntaxNode::on_serialize(context);
  for (auto& node : nodes_) {
    context.add_element("node", *node);
  }
}

#endif
