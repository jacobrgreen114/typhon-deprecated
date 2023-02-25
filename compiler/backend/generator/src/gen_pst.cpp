// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#include "gen_pst.hpp"

constexpr auto root_node_name     = std::string_view{"SymbolTable"};
constexpr auto ns_node_name       = std::string_view{"Namespace"};

constexpr auto cinclude_node_name = std::string_view{"CInclude"};
constexpr auto ctype_node_name    = std::string_view{"CType"};

constexpr auto var_node_name      = std::string_view{"Variable"};
constexpr auto struct_node_name   = std::string_view{"Struct"};

constexpr auto proj_attr_name     = std::string_view{"project"};
constexpr auto name_attr_name     = std::string_view{"name"};
constexpr auto type_attr_name     = std::string_view{"type"};
constexpr auto ctype_attr_name    = std::string_view{"ctype"};

auto create_node(xml::document& doc, const CInclude& include) -> xml::node& {
  auto& node = xml::allocate_element(doc, cinclude_node_name);
  node.append_attribute(&xml::allocate_attribute(doc, name_attr_name, include.name()));
  return node;
}

auto create_node(xml::document& doc, const CTypeDefinition& ctype) -> xml::node& {
  auto& node = xml::allocate_element(doc, ctype_node_name);
  node.append_attribute(&xml::allocate_attribute(doc, name_attr_name, ctype.name()));
  node.append_attribute(&xml::allocate_attribute(doc, ctype_attr_name, ctype.c_name()));
  return node;
}

auto create_node(xml::document& doc, const VariableDefinition& var) -> xml::node& {
  auto& node = xml::allocate_element(doc, var_node_name);
  node.append_attribute(&xml::allocate_attribute(doc, name_attr_name, var.name()));
  node.append_attribute(&xml::allocate_attribute(doc, type_attr_name, var.type_name()));
  return node;
}

auto create_node(xml::document& doc, const StructDefinition& strct) -> xml::node& {
  auto& node = xml::allocate_element(doc, struct_node_name);
  node.append_attribute(&xml::allocate_attribute(doc, name_attr_name, strct.name()));
  return node;
}

auto append_cincludes(xml::document& doc, xml::node& node, const SyntaxTree& tree) {
  for (auto& pinclude : tree.cincludes()) {
    auto& include = deref(pinclude);
    if (include.access() >= AccessModifier::Public) {
      node.append_node(&create_node(doc, include));
    }
  }
}

auto append_ctypes(xml::document& doc, xml::node& node, const SyntaxTree& tree) {
  for (auto& ptype : tree.ctypes()) {
    auto& ctype = deref(ptype);
    if (ctype.access() >= AccessModifier::Public) {
      node.append_node(&create_node(doc, ctype));
    }
  }
}

auto append_variables(xml::document& doc, xml::node& node, const SyntaxTree& tree) {
  for (auto& pvariable : tree.variables()) {
    auto& variable = deref(pvariable);
    if (variable.access() >= AccessModifier::Public) {
      node.append_node(&create_node(doc, variable));
    }
  }
}

auto append_structs(xml::document& doc, xml::node& node, const SyntaxTree& tree) {
  for (auto& pstruct : tree.structs()) {
    auto& strct = deref(pstruct);
    if (strct.access() >= AccessModifier::Public) {
      node.append_node(&create_node(doc, strct));
    }
  }
}

auto append_tree(xml::document& doc, xml::node& node, const SyntaxTree& tree) {
  append_cincludes(doc, node, tree);
  append_ctypes(doc, node, tree);
  append_variables(doc, node, tree);
  append_structs(doc, node, tree);
}

auto create_namespace(xml::document& doc, const NameSpace& ns) -> xml::node& {
  auto& node = xml::allocate_element(doc, ns_node_name);

  if (!ns.name().empty()) {
    node.append_attribute(&xml::allocate_attribute(doc, name_attr_name, ns.name()));
  }

  for (auto& ptree : ns.trees()) {
    append_tree(doc, node, deref(ptree));
  }

  for (auto& pns : ns.sub_spaces()) {
    node.append_node(&create_namespace(doc, deref(pns)));
  }

  return node;
}

auto generate_public_symbol_table(const ProjectConfig& config, const ProjectTree& tree) -> void {
  auto doc   = xml::document{};

  auto& root = xml::allocate_element(doc, root_node_name);
  root.append_attribute(&xml::allocate_attribute(doc, proj_attr_name, config.name()));

  root.append_node(&create_namespace(doc, deref(tree.root())));

  doc.append_node(&root);

  auto pub_sym_path = config.dir_binary() / config.name() += ".tysym";
  auto stream                                             = std::ofstream{pub_sym_path};
  stream << doc;
}
