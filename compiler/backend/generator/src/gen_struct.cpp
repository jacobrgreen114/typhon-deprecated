// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_struct.hpp"

#include "gen_var.hpp"

auto write_declaration(std::ostream& writer, const StructDefinition& def) -> void {
  writer << "class " << def.name();
}

auto write_forward_decl(std::ostream& writer, const StructDefinition& def) -> void {
  write_declaration(writer, def);
  writer << ';';
}

auto write_struct_definition(std::ostream& writer, const StructDefinition& str) -> void {
  write_declaration(writer, str);
  writer << " {" << newline;

  for (auto& def : str.definitions()) {
    switch (def->kind()) {
      case SyntaxKind::DefVar: {
        write_def(writer, *ptr_cast<VarDefinition>(def.get()));
        break;
      }
      case SyntaxKind::DefFunc: {
        break;
        //throw_not_implemented();
      }
      case SyntaxKind::DefStruct: {
        break;
        //throw_not_implemented();
      }

      default:
        throw_not_implemented();
    }
    writer << newline;
  }

  writer << "};" << newline;
}