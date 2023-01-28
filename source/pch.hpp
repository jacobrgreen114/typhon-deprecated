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

namespace fs     = std::filesystem;
namespace chrono = std::chrono;

#ifndef NODISCARD
#define NODISCARD [[nodiscard]]
#endif

#include "state_machine.hpp"
#include "xml/serialization.hpp"
#include "lexical_analysis/token.hpp"

// #ifdef _DEBUG
// #define assert(expr)                           \
//   if (!(expr)) {                               \
//     throw std::exception("assertion failed!"); \
//   }
// #endif

//#define __DEFINE_ENUM_CLASS(type_name, ...) enum class type_name { hello##__VA_ARGS__ }

#define ENUM_NAME(enum_type, enum_name) \
  { enum_type::enum_name, #enum_name }

// #define __ENUM_NAMES(enum_type, enum_name, ...) \
//   __ENUM_NAME(enum_type, enum_name)             \
//   __VA_OPT__(__ENUM_NAMES(enum_type, ##__VA_ARGS__))
//
// #define __DEFINE_ENUM_NAMES(type_name, ...)                                         \
//   const auto type_name##_names_ = std::unordered_map<type_name, std::string_view> { \
//     __ENUM_NAMES(type_name, ##__VA_ARGS__)                                          \
//   }
//
// #define DEFINE_ENUM(type_name, ...)              \
//   __DEFINE_ENUM_CLASS(type_name, ##__VA_ARGS__); \
//   __DEFINE_ENUM_NAMES(type_name, ##__VA_ARGS__);
//
// DEFINE_ENUM(TestKind, Source, Hello, World);