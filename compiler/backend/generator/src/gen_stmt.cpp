// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_stmt.hpp"

#include "gen_expr.hpp"
#include "gen_var.hpp"

auto write_statement(std::ostream& writer, const std::unique_ptr<BaseStatement>& statement) -> void;
auto write_block(std::ostream& writer, const StatementBlock& body) -> void;

auto write_stmt_def(std::ostream& writer, const DefinitionStatement& stmt) {
  auto& def = deref(stmt.def());

  switch (def.kind()) {
    case SyntaxKind::DefVar: {
      write_def(writer, ref_cast<VariableDefinition>(def));
      break;
    }
    default: {
      throw_not_implemented();
    }
  }
}

auto write_stmt_expr(std::ostream& writer, const ExpressionStatement& stmt) {
  write_expression(writer, stmt.expr());
  writer << ';' << newline;
}

auto write_stmt_ret(std::ostream& writer, const ReturnStatement& stmt) {
  writer << "return ";
  write_expression(writer, stmt.expr());
  writer << ';' << newline;
}

auto write_stmt_if(std::ostream& writer, const IfStatement& stmt) {
  writer << "if (";
  write_expression(writer, stmt.expr());
  writer << ") ";
  write_block(writer, deref(stmt.body()));
}

auto write_stmt_elif(std::ostream& writer, const ElifStatement& stmt) {
  writer << "else if (";
  write_expression(writer, stmt.expr());
  writer << ") ";
  write_block(writer, deref(stmt.body()));
}

auto write_stmt_else(std::ostream& writer, const ElseStatement& stmt) {
  writer << "else ";
  write_block(writer, deref(stmt.body()));
}

auto write_stmt_loop(std::ostream& writer, const LoopStatement& stmt) {
  writer << "while (true) ";
  write_block(writer, deref(stmt.body()));
}

auto write_stmt_while(std::ostream& writer, const WhileStatement& stmt) {
  writer << "while (";
  write_expression(writer, stmt.expr());
  writer << ") ";
  write_block(writer, deref(stmt.body()));
}

auto write_stmt_for(std::ostream& writer, const ForStatement& stmt) {
  writer << "for (";
  write_statement(writer, stmt.prefix());
  writer << ' ';
  write_expression(writer, stmt.cond());
  writer << "; ";
  write_expression(writer, stmt.postfix());
  writer << ") ";
  write_block(writer, *stmt.body());
}

auto write_statement(std::ostream& writer, const std::unique_ptr<BaseStatement>& statement)
    -> void {
  auto& stmt = deref(statement);

  switch (stmt.kind()) {
    case SyntaxKind::StmtDef: {
      write_stmt_def(writer, ref_cast<DefinitionStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtExpr: {
      write_stmt_expr(writer, ref_cast<ExpressionStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtRet: {
      write_stmt_ret(writer, ref_cast<ReturnStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtIf: {
      write_stmt_if(writer, ref_cast<IfStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtElif: {
      write_stmt_elif(writer, ref_cast<ElifStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtElse: {
      write_stmt_else(writer, ref_cast<ElseStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtLoop: {
      write_stmt_loop(writer, ref_cast<LoopStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtWhile: {
      write_stmt_while(writer, ref_cast<WhileStatement>(stmt));
      break;
    }
    case SyntaxKind::StmtFor: {
      write_stmt_for(writer, ref_cast<ForStatement>(stmt));
      break;
    }
    default:
      throw_not_implemented();
  }
}

auto write_block(std::ostream& writer, const StatementBlock& body) -> void {
  writer << " {";

  if (!body.statements().empty()) {
    writer << newline;
    for (auto& statement : body.statements()) {
      write_statement(writer, statement);
    }
    writer << '}';
  } else {
    writer << " }";
  }
}