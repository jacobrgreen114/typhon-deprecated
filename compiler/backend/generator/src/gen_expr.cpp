// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_expr.hpp"

/*
 * Expressions
 */

auto write_expr_bool(std::ostream& writer, const BoolExpression& expr) -> void {
  writer << (expr.value() ? "true" : "false");
}

auto write_expr_number(std::ostream& writer, const NumberExpression& expr) -> void {
  writer << expr.value();
}

auto write_expr_string(std::ostream& writer, const StringExpression& expr) -> void {
  writer << '"' << expr.value() << '"';
}

auto write_expr_ident(std::ostream& writer, const IdentifierExpression& expr) -> void {
  writer << expr.identifier();
}

auto write_expr_call(std::ostream& writer, const CallExpression& expr) -> void {
  writer << expr.identifier() << '(';
  auto& params = expr.parameters();
  if (!params.empty()) {
    write_expression(writer, params[0]);
    for (auto it = ++params.begin(); it != params.end(); ++it) {
      writer << ", ";
      write_expression(writer, *it);
    }
  }

  writer << ')';
}

auto write_expression(std::ostream& writer, const std::unique_ptr<ExpressionNode>& expr) -> void;

constexpr auto is_no_space_op(Operator op) -> bool {
  return op == Operator::Access || op == Operator::Static;
}

auto write_expr_binary(std::ostream& writer, const BinaryExpression& expr) -> void {
  // writer << '(';
  write_expression(writer, expr.lhs());

  const auto op = expr.op();
  if (is_no_space_op(op)) {
    writer << get_operator_symbol(op);
  } else {
    writer << ' ' << get_operator_symbol(op) << ' ';
  }

  write_expression(writer, expr.rhs());
  // writer << ')';
}

auto write_expr_unary(std::ostream& writer, const UnaryExpression& expr) -> void {
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

auto write_expression(std::ostream& writer, const std::unique_ptr<ExpressionNode>& expr) -> void {
  switch (expr->kind()) {
    case SyntaxKind::ExprBool: {
      write_expr_bool(writer, deref(ptr_cast<BoolExpression>(expr.get())));
      break;
    }
    case SyntaxKind::ExprNumber: {
      write_expr_number(writer, deref(ptr_cast<NumberExpression>(expr.get())));
      break;
    }
    case SyntaxKind::ExprString: {
      write_expr_string(writer, deref(ptr_cast<StringExpression>(expr.get())));
      break;
    }
    case SyntaxKind::ExprIdentifier: {
      write_expr_ident(writer, deref(ptr_cast<IdentifierExpression>(expr.get())));
      break;
    }
    case SyntaxKind::ExprCall: {
      write_expr_call(writer, deref(ptr_cast<CallExpression>(expr.get())));
      break;
    }
    case SyntaxKind::ExprUnary: {
      write_expr_unary(writer, deref(ptr_cast<UnaryExpression>(expr.get())));
      break;
    }
    case SyntaxKind::ExprBinary: {
      write_expr_binary(writer, deref(ptr_cast<BinaryExpression>(expr.get())));
      break;
    }
  }
}