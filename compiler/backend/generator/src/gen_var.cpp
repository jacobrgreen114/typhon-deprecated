// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_var.hpp"

#include "gen_expr.hpp"

auto write_declaration(std::ostream& writer, const VariableDefinition& def) -> void {
  if (!def.is_mutable()) {
    writer << "const ";
  }

  if (def.is_typed()) {
    writer << identifer_prefix << def.type_name();
  } else {
    writer << keyword_auto;
  }

  writer << ' ' << identifer_prefix << def.name();
}

auto write_forward_decl(std::ostream& writer, const VariableDefinition& def) -> void {
  writer << "extern ";
  write_declaration(writer, def);
  writer << ';' << newline;
}

auto write_def(std::ostream& writer, const VariableDefinition& def) -> void {
  write_declaration(writer, def);

  if (def.assignment()) {
    writer << " = ";
    write_expression(writer, def.assignment());
  }

  writer << ';' << newline;
}
