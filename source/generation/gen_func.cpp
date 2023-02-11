// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_func.hpp"

#include "gen_stmt.hpp"

auto write_parameter(std::ostream& writer, const std::shared_ptr<FuncParameter>& param) {
  if (param->type_name()) {
    auto type_name = mutate_type_name(*param->type_name());
    writer << type_name;
  } else {
    writer << keyword_auto;
  }

  writer << " " << *param->name();
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

auto write_forward_decl(std::ostream& writer, const std::shared_ptr<FuncDefinition>& def) -> void {
  write_function_declaration(writer, def);
  writer << ';';
}

auto write_function_definition(std::ostream& writer, const std::shared_ptr<FuncDefinition>& def)
    -> void {
  write_function_declaration(writer, def);
  write_block(writer, def->body());
  writer << newline;
}