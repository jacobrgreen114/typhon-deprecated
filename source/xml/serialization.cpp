// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#include "serialization.hpp"

namespace xml {

SerializationContext::SerializationContext(std::ostream& writer,
                                           std::string_view name, size_t indent)
    : writer_{writer},
      element_name_{name},
      indent_{indent},
      attr_end_{false},
      content_{false},
      element_count_{0} {
  write_indent();
  writer_ << '<' << name;
}

SerializationContext::~SerializationContext() {
  if (is_empty()) {
    writer_ << "/>" << std::endl;
  } else {
    //terminate_open();
    if (element_count_ > 0) {
      write_indent();
    }
    writer_ << '<' << '/' << element_name_ << '>' << std::endl;
  }
}

auto SerializationContext::write_indent() -> void {
  for (size_t i = 0; i < indent_; ++i) {
    writer_ << '\t';
  }
}

auto SerializationContext::terminate_open() -> void {
  if (!attr_end_) {
    attr_end_ = true;
    writer_ << '>';
  }
}

auto SerializationContext::is_empty() const -> bool {
  return element_count_ == 0 && !content_;
}

auto SerializationContext::add_attribute(std::string_view name,
                                         std::string_view value) -> void {
  if (attr_end_) {
    throw std::exception();
  }
  writer_ << ' ' << name << '=' << '"' << value << '"';
}

auto SerializationContext::add_content(std::string_view value) -> void {
  content_ = true;
  terminate_open();
  if (element_count_ > 0) {
    throw std::exception();
  }
  writer_ << value;
}

auto SerializationContext::add_content(uintptr_t value) -> void {
  content_ = true;
  terminate_open();
  if (element_count_ > 0) {
    throw std::exception();
  }
  writer_ << value;
}
auto SerializationContext::add_content(intptr_t value) -> void {
  content_ = true;
  terminate_open();
  if (element_count_ > 0) {
    throw std::exception();
  }
  writer_ << value;
}

auto SerializationContext::add_element(std::string_view name,
                                       const Serializable& serializable)
    -> void {
  terminate_open();
  if (element_count_++ < 1) {
    writer_ << std::endl;
  }
  auto context = SerializationContext{writer_, name, indent_ + 1};
  serializable.on_serialize(context);
}

auto Serializable::on_serialize(SerializationContext& context) const -> void {}

auto Serializable::serialize(std::ostream& writer,
                             std::string_view root_name) const -> void {
  auto context = SerializationContext{writer, root_name};
  on_serialize(context);
  writer.flush();
}

}  // namespace xml