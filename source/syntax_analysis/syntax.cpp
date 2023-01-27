
// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "syntax.hpp"

constexpr auto to_string(SyntaxKind kind) -> const char* {
  switch (kind) {
    case SyntaxKind::Source:
      return "Source";

    case SyntaxKind::ExprNumber:
      return "ExprNumber";
    case SyntaxKind::ExprBinary:
      return "ExprBinary";

    case SyntaxKind::StmBlock:
      return "StmBlock";
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

constexpr auto to_string(Operator op) -> const char* {
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

    default:
      throw_not_implemented();
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
  context.add_attribute("value", *value_);
}

auto Operation::on_serialize(xml::SerializationContext& context) const -> void {
  SyntaxNode::on_serialize(context);
  context.add_attribute("op", to_string(op()));
}

void BinaryExpression::on_serialize(xml::SerializationContext& context) const {
  Operation::on_serialize(context);
  context.add_element("lhs", *lhs_);
  context.add_element("rhs", *rhs_);
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
