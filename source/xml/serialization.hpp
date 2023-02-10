// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "../common.hpp"

namespace xml {

class Serializable;

class SerializationContext final {
  std::ostream& writer_;
  std::string_view element_name_;
  size_t indent_;

  bool attr_end_;
  bool content_;
  size_t element_count_;

  auto write_indent() -> void;
  auto terminate_open() -> void;

  auto is_empty() const -> bool;

 public:
  explicit SerializationContext(std::ostream& writer, std::string_view name, size_t indent = 0);

  ~SerializationContext();

  template <typename T1, typename T2>
  auto add_attribute(const T1& name, const T2 value) {
    if (attr_end_) {
      throw std::exception();
    }
    writer_ << ' ' << name << '=' << '"' << value << '"';
  }

  auto add_attribute(std::string_view name, std::string_view value) -> void;
  auto add_content(std::string_view value) -> void;
  auto add_content(uintptr_t value) -> void;
  auto add_content(intptr_t value) -> void;
  auto add_element(std::string_view name, const Serializable& serializable) -> void;
};

class Serializable {
  friend SerializationContext;

 public:
  virtual ~Serializable() = default;

  auto serialize(std::ostream& writer, std::string_view root_name = "root") const -> void;

 protected:
  virtual auto on_serialize(SerializationContext& context) const -> void;
};

}  // namespace xml
