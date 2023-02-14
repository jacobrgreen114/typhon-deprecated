
// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include <array>
#include <exception>
#include <fstream>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include <filesystem>
#include <iostream>
#include <span>
#include <chrono>
#include <unordered_set>
#include <set>
#include <cassert>

namespace fs     = std::filesystem;
namespace chrono = std::chrono;

#ifndef NODISCARD
#define NODISCARD [[nodiscard]]
#endif

#define throw_not_implemented() throw std::exception("not implemented!")

#ifndef NDEBUG
template <typename To, typename From>
constexpr auto ptr_cast(std::unique_ptr<From> from) -> std::unique_ptr<To> {
  if (from == nullptr) {
    return nullptr;
  }
  auto temp = dynamic_cast<To*>(from.release());
  assert(temp != nullptr);
  return std::unique_ptr<To>(temp);
}

template <typename To, typename From>
constexpr auto ptr_cast(std::shared_ptr<From> from) -> std::shared_ptr<To> {
  if (from == nullptr) {
    return nullptr;
  }
  auto temp = std::dynamic_pointer_cast<To>(from);
  assert(temp != nullptr);
  return std::unique_ptr<To>(temp);
}

template <typename To, typename From>
constexpr auto ptr_cast(From* from) -> To* {
  if (from == nullptr) {
    return nullptr;
  }
  auto temp = dynamic_cast<To*>(from);
  assert(temp != nullptr);
  return temp;
}

template <typename T>
constexpr auto& deref(T* t) {
  assert(t != nullptr);
  return *t;
}

#else

template <typename To, typename From>
constexpr auto ptr_cast(std::unique_ptr<From> from) -> std::unique_ptr<To> {
  return std::unique_ptr<To>(static_cast<To*>(from.release()));
}

template <typename To, typename From>
constexpr auto ptr_cast(std::shared_ptr<From> from) -> std::shared_ptr<To> {
  return std::static_pointer_cast<To>(from);
}

template <typename To, typename From>
constexpr auto ptr_cast(From* from) -> To* {
  return static_cast<To*>(from);
}

template <typename T>
constexpr auto& deref(T* t) {
  return *t;
}

#endif

template <typename T>
constexpr auto& deref(const std::unique_ptr<T>& t) {
  return deref(t.get());
}

template <typename T>
constexpr auto& deref(const std::shared_ptr<T>& t) {
  return deref(t.get());
}

template <typename To, typename From>
constexpr auto ref_cast(From& from) -> To& {
  return *static_cast<To*>(&from);
}

inline std::ostream& newline(std::ostream& os) {
  os.put(os.widen('\n'));
  return os;
}

inline std::ostream& newline(std::ostream& os, uint64_t count) {
  for (auto i = 0; i < count; ++i) {
    os.put(os.widen('\n'));
  }
  return os;
}

#ifdef TRACE
#define TRACE_PRINT(str) std::cout << "[TRACE] " << str
#else
#define TRACE_PRINT(str) ((void)0)
#endif
