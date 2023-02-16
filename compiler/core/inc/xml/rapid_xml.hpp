// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

// todo : fork RapidXML and add c++ features

#include "../common.hpp"
#include "../../../../external/RapidXML/RapidXML/rapidxml.hpp"

namespace rapidxml {
namespace internal {
template <class OutIt, class Ch>
inline OutIt print_children(OutIt out, const xml_node<Ch>* node, int flags, int indent);

template <class OutIt, class Ch>
inline OutIt print_attributes(OutIt out, const xml_node<Ch>* node, int flags);

template <class OutIt, class Ch>
inline OutIt print_data_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

template <class OutIt, class Ch>
inline OutIt print_cdata_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

template <class OutIt, class Ch>
inline OutIt print_element_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

template <class OutIt, class Ch>
inline OutIt print_declaration_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

template <class OutIt, class Ch>
inline OutIt print_comment_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

template <class OutIt, class Ch>
inline OutIt print_doctype_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);

template <class OutIt, class Ch>
inline OutIt print_pi_node(OutIt out, const xml_node<Ch>* node, int flags, int indent);
}  // namespace internal
}  // namespace rapidxml

#include "../../../../external/RapidXML/RapidXML/rapidxml_print.hpp"

namespace xml {
using document = rapidxml::xml_document<>;
using node     = rapidxml::xml_node<>;
using attr     = rapidxml::xml_attribute<>;

inline auto& allocate_element(document& doc, std::string_view name, std::string_view value = {}) {
  return deref(doc.allocate_node(
      rapidxml::node_type::node_element, name.data(), value.data(), name.size(), value.size()));
}

inline auto& allocate_attribute(document& doc, std::string_view name, std::string_view value) {
  return deref(doc.allocate_attribute(name.data(), value.data(), name.size(), value.size()));
}

inline auto allocate_string(document& doc, std::string_view value) {
  return std::string_view{doc.allocate_string(value.data(), value.size()), value.size()};
}

inline auto get_node_name(const node& node) {
  return std::string_view{node.name(), node.name_size()};
}

inline auto get_node_value(const node& node) {
  return std::string_view{node.value(), node.value_size()};
}

}  // namespace xml