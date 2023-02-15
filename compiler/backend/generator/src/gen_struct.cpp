// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_struct.hpp"

#include "gen_var.hpp"
#include "gen_func.hpp"
#include "gen_object.hpp"

auto write_declaration(std::ostream& writer, const StructDefinition& def) -> void {
  writer << "class " << identifer_prefix << def.name();
}

auto write_forward_decl(std::ostream& writer, const StructDefinition& def) -> void {
  write_declaration(writer, def);
  writer << ';';
}

auto write_struct_definition(std::ostream& writer, const StructDefinition& str) -> void {
  write_declaration(writer, str);
  writer << " {" << newline;

  for (auto& var : str.variables()) {
    write_def(writer, deref(var));
  }

  for (auto& func : str.functions()) {
    write_forward_decl(writer, deref(func));
  }

  for (auto& strct : str.structs()) {
    writer << newline;
    write_struct_definition(writer, deref(strct));
  }

  for (auto& object : str.objects()) {
    writer << newline;
    write_object_definition(writer, deref(object));
  }

  writer << "};" << newline;
}