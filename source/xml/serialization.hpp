// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

namespace xml {

class Serializable;

class SerializationContext final {
  std::ostream& writer_;
  const char* element_name_;
  size_t indent_;

  bool attr_end_;
  size_t element_count_;

  auto write_indent() -> void;
  auto terminate_open() -> void;

 public:
  explicit SerializationContext(std::ostream& writer, const char* name,
                                size_t indent = 0);

  ~SerializationContext();

  auto add_attribute(const char* name, const char* value) -> void;
  auto add_content(const char* value) -> void;
  auto add_content(uintptr_t value) -> void;
  auto add_content(intptr_t value) -> void;
  auto add_element(const char* name, const Serializable& serializable) -> void;
};

class Serializable {
  friend SerializationContext;

 public:
  virtual ~Serializable() = default;

  auto serialize(std::ostream& writer, const char* root_name = "root") const
      -> void;

 protected:
  virtual auto on_serialize(SerializationContext& context) const -> void;
};

}  // namespace xml
