// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_object.hpp"

#include "gen_var.hpp"
#include "gen_func.hpp"
#include "gen_struct.hpp"

auto write_declaration(std::ostream& writer, const ObjectDefinition& def) -> void {
  writer << "class " << def.name();
}

auto write_forward_decl(std::ostream& writer, const ObjectDefinition& def) -> void {
  write_declaration(writer, def);
  writer << ';';
}

auto write_object_definition(std::ostream& writer, const ObjectDefinition& str) -> void {
  write_declaration(writer, str);
  writer << " {";

  for (auto& var : str.variables()) {
    writer << newline;
    write_def(writer, deref(var));
  }

  for (auto& func : str.functions()) {
    writer << newline;
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

  writer << newline << "};" << newline;
}