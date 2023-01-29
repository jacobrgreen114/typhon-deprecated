// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "generator.hpp"

#include "../paths.hpp"

auto write_expression(std::ostream& writer, const std::shared_ptr<ExpressionNode>& expr) -> void;

constexpr auto keyword_auto = std::string_view{"auto"};

const auto c_type_map       = std::unordered_map<std::string_view, std::string_view>{
    {"bool32", "uint8_t" },

    {"u8",     "uint8_t" },
    {"u16",    "uint16_t"},
    {"u32",    "uint32_t"},
    {"u64",    "uint64_t"},

    {"i8",     "int8_t"  },
    {"i16",    "int16_t" },
    {"i32",    "int32_t" },
    {"i64",    "int64_t" },
};

auto mutate_type_name(std::string_view view) -> std::string_view {
  if (auto c_type = c_type_map.find(view); c_type != c_type_map.end()) {
    return c_type->second;
  }
  return view;
}

const auto operator_symbol_map = std::unordered_map<Operator, std::string_view>{
    {Operator::Access,            "." },
    {Operator::Assign,            "=" },

    {Operator::Add,               "+" },
    {Operator::Subtract,          "-" },
    {Operator::Multiply,          "*" },
    {Operator::Divide,            "/" },

    {Operator::Equals,            "=="},
    {Operator::NotEquals,         "!="},
    {Operator::Or,                "||"},
    {Operator::And,               "&&"},

    {Operator::LessThan,          "<" },
    {Operator::GreaterThan,       ">" },
    {Operator::LessThanEquals,    "<="},
    {Operator::GreaterThanEquals, ">="},

    {Operator::BitOr,             "|" },
    {Operator::BitXor,            "^" },
    {Operator::BitAnd,            "&" },

    {Operator::ShiftLeft,         "<<"},
    {Operator::ShiftRight,        ">>"},

    {Operator::BoolNot,           "!" },
    {Operator::BitNot,            "~" },

    {Operator::Positive,          "+" },
    {Operator::Negative,          "-" },

    {Operator::PreInc,            "++"},
    {Operator::PreDec,            "--"},

    {Operator::PostInc,           "++"},
    {Operator::PostDec,           "--"},
};

auto get_operator_symbol(Operator op) -> std::string_view {
  const auto sym = operator_symbol_map.find(op);
  if (sym == operator_symbol_map.end()) {
    throw std::exception();
  }
  return sym->second;
}

auto forward_declare(std::ostream& writer, const std::shared_ptr<VarDefinition>& def) -> void {
  auto type_name = def->type_name() ? mutate_type_name(*def->type_name()) : keyword_auto;
  auto name      = *def->name();

  writer << type_name << ' ' << name << ';' << std::endl;
}

auto write_parameter(std::ostream& writer, const std::shared_ptr<FuncParameter>& param) {
  writer << *param->name();
  if (param->type_name()) {
    auto type_name = mutate_type_name(*param->type_name());
    writer << " : " << type_name;
  }
}

auto write_parameter_block(std::ostream& writer, const std::shared_ptr<FuncDefinition>& def)
    -> void {
  writer << '(';

  const auto& parameters = def->parameters();
  const auto last_index  = parameters.size() - 1;
  for (size_t index = 0; index < parameters.size(); ++index) {
    auto& param = parameters[index];
    write_parameter(writer, param);
    if (index < last_index) {
      writer << ", ";
    }
  }

  writer << ')';
}

auto write_function_declaration(std::ostream& writer, const std::shared_ptr<FuncDefinition>& def) {
  writer << keyword_auto << ' ' << *def->name();

  write_parameter_block(writer, def);

  if (def->return_type()) {
    auto ret_type_name = mutate_type_name(*def->return_type());
    writer << " -> " << ret_type_name;
  }
}

auto write_def(std::ostream& writer, const std::shared_ptr<VarDefinition>& def) {
  auto type_name = def->type_name() ? mutate_type_name(*def->type_name()) : keyword_auto;
  auto name      = *def->name();

  writer << type_name << ' ' << name;

  if (def->assignment()) {
    writer << " = ";
    write_expression(writer, def->assignment());
  }

  writer << ';';
}

auto write_decl(std::ostream& writer, const std::shared_ptr<FuncDefinition>& def) -> void {
  write_function_declaration(writer, def);
  writer << ';';
}

auto forward_decl_structs(std::ostream& writer, const SyntaxTree::NodeArray& nodes) {
  for (auto& node : nodes) {
    if (node->kind() == SyntaxKind::DefStruct) {
      // todo : implement
    }
  }
}

auto forward_decl_objects(std::ostream& writer, const SyntaxTree::NodeArray& nodes) {
  auto empty = true;
  for (auto& node : nodes) {
    if (node->kind() == SyntaxKind::DefObject) {
      empty = false;
      // todo : implement
    }
  }

  if (!empty) {
    writer << std::endl;
  }
}

auto forward_decl_aliases(std::ostream& writer, const SyntaxTree::NodeArray& nodes) {
  auto empty = true;

  for (auto& node : nodes) {
    if (node->kind() == SyntaxKind::DefObject) {
      empty = false;
      // todo : implement
    }
  }

  if (!empty) {
    writer << std::endl;
  }
}

auto forward_declare_funcs(std::ostream& writer, const SyntaxTree::NodeArray& nodes) {
  auto empty = true;

  for (auto& node : nodes) {
    if (node->kind() == SyntaxKind::DefFunc) {
      empty = false;
      write_decl(writer, ptr_cast<FuncDefinition>(node));
      writer << std::endl;
    }
  }

  if (!empty) {
    writer << std::endl;
  }
}

auto forward_declare_vars(std::ostream& writer, const SyntaxTree::NodeArray& nodes) {
  auto empty = true;

  for (auto& node : nodes) {
    if (node->kind() == SyntaxKind::DefVar) {
      empty = false;
      write_def(writer, ptr_cast<VarDefinition>(node));
      writer << std::endl;
    }
  }

  if (!empty) {
    writer << std::endl;
  }
}

auto forward_declare(std::ostream& writer, const std::shared_ptr<SyntaxTree>& source) -> void {
  writer << "/*" << std::endl
         << " *  Forward Declarations" << std::endl
         << " */" << std::endl
         << std::endl;

  auto& nodes = source->nodes();
  forward_decl_structs(writer, nodes);
  forward_decl_objects(writer, nodes);
  forward_decl_aliases(writer, nodes);
  forward_declare_funcs(writer, nodes);
  forward_declare_vars(writer, nodes);
}

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

auto write_expr_string(std::ostream& writer, const std::shared_ptr<StringExpression>& expr)
    -> void {
  writer << *expr->value();
}

auto write_expr_ident(std::ostream& writer, const std::shared_ptr<IdentifierExpression>& expr)
    -> void {
  writer << *expr->identifier();
}

auto write_expression(std::ostream& writer, const std::shared_ptr<ExpressionNode>& expr) -> void;

constexpr auto is_no_space_op(Operator op) -> bool { return op == Operator::Access; }

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
      write_expr_string(writer, ptr_cast<StringExpression>(expr));
      break;
    }
    case SyntaxKind::ExprIdentifier: {
      write_expr_ident(writer, ptr_cast<IdentifierExpression>(expr));
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

/*
 * Statements
 */

auto write_statement(std::ostream& writer, const std::shared_ptr<Statement>& statement) -> void;
auto write_block(std::ostream& writer, const std::shared_ptr<StatementBlock>& body) -> void;

auto write_stmt_def(std::ostream& writer, const std::shared_ptr<DefStatement>& stmt) {
  switch (stmt->def()->kind()) {
    case SyntaxKind::DefVar: {
      write_def(writer, ptr_cast<VarDefinition>(stmt->def()));
      break;
    }
    default: {
      throw_not_implemented();
    }
  }
}

auto write_stmt_expr(std::ostream& writer, const std::shared_ptr<ExprStatement>& stmt) {
  write_expression(writer, stmt->expr());
  writer << ';';
}

auto write_stmt_ret(std::ostream& writer, const std::shared_ptr<ReturnStatement>& stmt) {
  writer << "return ";
  write_expression(writer, stmt->expr());
  writer << ';';
}

auto write_stmt_if(std::ostream& writer, const std::shared_ptr<IfStatement>& stmt) {
  writer << "if (";
  write_expression(writer, stmt->expr());
  writer << ") ";
  write_block(writer, stmt->body());
}

auto write_stmt_elif(std::ostream& writer, const std::shared_ptr<ElifStatement>& stmt) {
  writer << "else if (";
  write_expression(writer, stmt->expr());
  writer << ") ";
  write_block(writer, stmt->body());
}

auto write_stmt_else(std::ostream& writer, const std::shared_ptr<ElseStatement>& stmt) {
  writer << "else ";
  write_block(writer, stmt->body());
}

auto write_stmt_loop(std::ostream& writer, const std::shared_ptr<LoopStatement>& stmt) {
  writer << "while (true) ";
  write_block(writer, stmt->body());
}

auto write_stmt_while(std::ostream& writer, const std::shared_ptr<WhileStatement>& stmt) {
  writer << "while (";
  write_expression(writer, stmt->expr());
  writer << ") ";
  write_block(writer, stmt->body());
}

auto write_stmt_for(std::ostream& writer, const std::shared_ptr<ForStatement>& stmt) {
  writer << "for (";
  write_statement(writer, stmt->prefix());
  writer << ' ';
  write_expression(writer, stmt->cond());
  writer << "; ";
  write_expression(writer, stmt->postfix());
  writer << ") ";
  write_block(writer, stmt->body());
}

auto write_statement(std::ostream& writer, const std::shared_ptr<Statement>& statement) -> void {
  switch (statement->kind()) {
    case SyntaxKind::StmtDef: {
      write_stmt_def(writer, ptr_cast<DefStatement>(statement));
      break;
    }
    case SyntaxKind::StmtExpr: {
      write_stmt_expr(writer, ptr_cast<ExprStatement>(statement));
      break;
    }
    case SyntaxKind::StmtRet: {
      write_stmt_ret(writer, ptr_cast<ReturnStatement>(statement));
      break;
    }
    case SyntaxKind::StmtIf: {
      write_stmt_if(writer, ptr_cast<IfStatement>(statement));
      break;
    }
    case SyntaxKind::StmtElif: {
      write_stmt_elif(writer, ptr_cast<ElifStatement>(statement));
      break;
    }
    case SyntaxKind::StmtElse: {
      write_stmt_else(writer, ptr_cast<ElseStatement>(statement));
      break;
    }
    case SyntaxKind::StmtLoop: {
      write_stmt_loop(writer, ptr_cast<LoopStatement>(statement));
      break;
    }
    case SyntaxKind::StmtWhile: {
      write_stmt_while(writer, ptr_cast<WhileStatement>(statement));
      break;
    }
    case SyntaxKind::StmtFor: {
      write_stmt_for(writer, ptr_cast<ForStatement>(statement));
      break;
    }
  }
}

auto write_block(std::ostream& writer, const std::shared_ptr<StatementBlock>& body) -> void {
  writer << " {";
  if (body->statements().empty()) {
    writer << " }";
    return;
  }
  writer << std::endl;
  for (auto& statement : body->statements()) {
    write_statement(writer, statement);
    writer << std::endl;
  }

  writer << '}';
}

auto write_function_definition(std::ostream& writer, const std::shared_ptr<FuncDefinition>& def)
    -> void {
  write_function_declaration(writer, def);
  write_block(writer, def->body());
  writer << std::endl;
}

auto write_definitions(std::ostream& writer, const std::shared_ptr<SyntaxTree>& source) -> void {
  for (auto& node : source->nodes()) {
    switch (node->kind()) {
      case SyntaxKind::DefFunc: {
        write_function_definition(writer, ptr_cast<FuncDefinition>(node));
        break;
      }
    }
  }
}

auto write_source_header(std::ostream& writer, const fs::path& rel_path) -> void {
  writer << "/*" << std::endl
         << " *  Generated by Typhon Compiler" << std::endl
         << " *      Source File : " << rel_path << std::endl
         << " */" << std::endl
         << std::endl;
}

const auto includes = std::vector<std::string_view>{"<cstdint>"};

auto write_includes(std::ostream& writer) {
  for (auto& include : includes) {
    writer << "#include " << include << std::endl;
  }
  if (!includes.empty()) {
    writer << std::endl;
  }
}

auto generate_source_file(const fs::path& rel_path, const std::shared_ptr<SyntaxTree>& source)
    -> void {
  auto src_file_path = rel_src_path_to_gen_src_file_path(rel_path);
  fs::create_directories(src_file_path.parent_path());

  auto writer = std::ofstream{src_file_path};

  write_source_header(writer, rel_path);
  write_includes(writer);
  forward_declare(writer, source);
  write_definitions(writer, source);
}

auto generate(const fs::path& rel_path, const std::shared_ptr<SyntaxTree>& source) -> void {
  TRACE_TIMER("generate");
  generate_source_file(rel_path, source);
}
