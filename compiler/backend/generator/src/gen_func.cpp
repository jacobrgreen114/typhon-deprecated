// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_func.hpp"

#include "gen_stmt.hpp"

auto write_parameter(std::ostream& writer, const FunctionParameter& param) {
  if (param.is_typed()) {
    writer << identifer_prefix << param.type_name();
  } else {
    writer << keyword_auto;
  }

  writer << " " << param.name();
}

auto write_parameter_block(std::ostream& writer, const FunctionDefinition& def) -> void {
  writer << '(';

  const auto& parameters = def.parameters();
  const auto last_index  = parameters.size() - 1;
  for (size_t index = 0; index < parameters.size(); ++index) {
    auto& param = parameters[index];
    write_parameter(writer, deref(param));
    if (index < last_index) {
      writer << ", ";
    }
  }

  writer << ')';
}

auto write_declaration(std::ostream& writer, const FunctionDefinition& def) {
  writer << keyword_auto << ' ' << identifer_prefix << def.name();

  write_parameter_block(writer, def);

  if (def.is_return_typed()) {
    writer << " -> " << identifer_prefix << def.return_type();
  }
}

auto write_forward_decl(std::ostream& writer, const FunctionDefinition& def) -> void {
  write_declaration(writer, def);
  writer << ';';
}

auto write_definition(std::ostream& writer, const FunctionDefinition& def) -> void {
  write_declaration(writer, def);
  write_block(writer, deref(def.body()));
  writer << newline;
}