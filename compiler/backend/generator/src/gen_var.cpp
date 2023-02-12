// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_var.hpp"

#include "gen_expr.hpp"

auto forward_declare(std::ostream& writer, const std::shared_ptr<VarDefinition>& def) -> void {
  auto type_name = !def->type_name().empty() ? mutate_type_name(def->type_name()) : keyword_auto;
  auto name      = def->name();

  writer << type_name << ' ' << name << ';' << newline;
}

auto write_def(std::ostream& writer, const std::shared_ptr<VarDefinition>& def) -> void {
  auto type_name = !def->type_name().empty() ? mutate_type_name(def->type_name()) : keyword_auto;
  auto name      = def->name();

  writer << type_name << ' ' << name;

  if (def->assignment()) {
    writer << " = ";
    write_expression(writer, def->assignment());
  }

  writer << ';';
}
