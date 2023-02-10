// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_expr.hpp"

/*
 * Expressions
 */

auto write_expr_bool(std::ostream& writer, const std::shared_ptr<BoolExpression>& expr) -> void {
  writer << (expr->value() ? "true" : "false");
}

auto write_expr_number(std::ostream& writer, const std::shared_ptr<NumberExpression>& expr)
    -> void {
  writer << *expr->value();
}

auto write_expr_string(std::ostream& writer, const std::shared_ptr<NumberExpression>& expr)
    -> void {
  writer << *expr->value();
}

auto write_expr_string(std::ostream& writer, const std::shared_ptr<ValueExpression>& expr) -> void {
  writer << *expr->value();
}

auto write_expr_ident(std::ostream& writer, const std::shared_ptr<IdentifierExpression>& expr)
    -> void {
  writer << *expr->identifier();
}

auto write_expression(std::ostream& writer, const std::shared_ptr<ExpressionNode>& expr) -> void;

constexpr auto is_no_space_op(Operator op) -> bool {
  return op == Operator::Access || op == Operator::Static;
}

auto write_expr_binary(std::ostream& writer, const std::shared_ptr<BinaryExpression>& expr)
    -> void {
  // writer << '(';
  write_expression(writer, expr->lhs());

  const auto op = expr->op();
  if (is_no_space_op(op)) {
    writer << get_operator_symbol(op);
  } else {
    writer << ' ' << get_operator_symbol(op) << ' ';
  }

  write_expression(writer, expr->rhs());
  // writer << ')';
}

auto write_expr_unary(std::ostream& writer, const std::shared_ptr<UnaryExpression>& expr) -> void {
  //write_expression(writer, expr->lhs());
  //
  //const auto op = expr->op();
  //
  //if (is_no_space_op(op)) {
  //  writer << get_operator_symbol(op);
  //} else {
  //  writer << ' ' << get_operator_symbol(op) << ' ';
  //}
  //
  //write_expression(writer, expr->rhs());
  throw_not_implemented();
}

auto write_expression(std::ostream& writer, const std::shared_ptr<ExpressionNode>& expr) -> void {
  switch (expr->kind()) {
    case SyntaxKind::ExprBool: {
      write_expr_bool(writer, ptr_cast<BoolExpression>(expr));
      break;
    }
    case SyntaxKind::ExprNumber: {
      write_expr_number(writer, ptr_cast<NumberExpression>(expr));
      break;
    }
    case SyntaxKind::ExprString: {
      write_expr_string(writer, ptr_cast<ValueExpression>(expr));
      break;
    }
    case SyntaxKind::ExprIdentifier: {
      write_expr_ident(writer, ptr_cast<IdentifierExpression>(expr));
      break;
    }
    case SyntaxKind::ExprCall: {
      // todo
      break;
    }
    case SyntaxKind::ExprUnary: {
      write_expr_unary(writer, ptr_cast<UnaryExpression>(expr));
      break;
    }
    case SyntaxKind::ExprBinary: {
      write_expr_binary(writer, ptr_cast<BinaryExpression>(expr));
      break;
    }
  }
}