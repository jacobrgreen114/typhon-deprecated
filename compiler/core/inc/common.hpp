
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

namespace fs     = std::filesystem;
namespace chrono = std::chrono;

#ifndef NODISCARD
#define NODISCARD [[nodiscard]]
#endif

#define throw_not_implemented() throw std::exception("not implemented!")

#if _DEBUG
template <typename To, typename From>
constexpr auto ptr_cast(const std::shared_ptr<From>& from) -> std::shared_ptr<To> {
  return std::dynamic_pointer_cast<To>(from);
}
#else
template <typename To, typename From>
constexpr auto ptr_cast(const std::shared_ptr<From>& from) -> std::shared_ptr<To> {
  return std::reinterpret_pointer_cast<To>(from);
}
#endif

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
