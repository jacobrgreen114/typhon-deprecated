// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_stmt.hpp"

#include "gen_expr.hpp"
#include "gen_var.hpp"

auto write_statement(std::ostream& writer, const std::unique_ptr<Statement>& statement) -> void;
auto write_block(std::ostream& writer, const StatementBlock& body) -> void;

auto write_stmt_def(std::ostream& writer, const DefStatement& stmt) {
  switch (stmt.def()->kind()) {
    case SyntaxKind::DefVar: {
      write_def(writer, *ptr_cast<VarDefinition>(stmt.def().get()));
      break;
    }
    default: {
      throw_not_implemented();
    }
  }
}

auto write_stmt_expr(std::ostream& writer, const ExprStatement& stmt) {
  write_expression(writer, stmt.expr());
  writer << ';';
}

auto write_stmt_ret(std::ostream& writer, const ReturnStatement& stmt) {
  writer << "return ";
  write_expression(writer, stmt.expr());
  writer << ';';
}

auto write_stmt_if(std::ostream& writer, const IfStatement& stmt) {
  writer << "if (";
  write_expression(writer, stmt.expr());
  writer << ") ";
  write_block(writer, *stmt.body());
}

auto write_stmt_elif(std::ostream& writer, const ElifStatement& stmt) {
  writer << "else if (";
  write_expression(writer, stmt.expr());
  writer << ") ";
  write_block(writer, *stmt.body());
}

auto write_stmt_else(std::ostream& writer, const ElseStatement& stmt) {
  writer << "else ";
  write_block(writer, *stmt.body());
}

auto write_stmt_loop(std::ostream& writer, const LoopStatement& stmt) {
  writer << "while (true) ";
  write_block(writer, *stmt.body());
}

auto write_stmt_while(std::ostream& writer, const WhileStatement& stmt) {
  writer << "while (";
  write_expression(writer, stmt.expr());
  writer << ") ";
  write_block(writer, *stmt.body());
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

auto write_statement(std::ostream& writer, const std::unique_ptr<Statement>& statement) -> void {
  switch (statement->kind()) {
    case SyntaxKind::StmtDef: {
      write_stmt_def(writer, *ptr_cast<DefStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtExpr: {
      write_stmt_expr(writer, *ptr_cast<ExprStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtRet: {
      write_stmt_ret(writer, *ptr_cast<ReturnStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtIf: {
      write_stmt_if(writer, *ptr_cast<IfStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtElif: {
      write_stmt_elif(writer, *ptr_cast<ElifStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtElse: {
      write_stmt_else(writer, *ptr_cast<ElseStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtLoop: {
      write_stmt_loop(writer, *ptr_cast<LoopStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtWhile: {
      write_stmt_while(writer, *ptr_cast<WhileStatement>(statement.get()));
      break;
    }
    case SyntaxKind::StmtFor: {
      write_stmt_for(writer, *ptr_cast<ForStatement>(statement.get()));
      break;
    }
  }
}

auto write_block(std::ostream& writer, const StatementBlock& body) -> void {
  writer << " {";
  if (body.statements().empty()) {
    writer << " }";
    return;
  }
  writer << newline;
  for (auto& statement : body.statements()) {
    write_statement(writer, statement);
    writer << newline;
  }

  writer << '}';
}