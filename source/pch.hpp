// Copyright (c) 2023. Jacob R. Green
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

namespace fs = std::filesystem;

#ifndef NODISCARD
#define NODISCARD [[nodiscard]]
#endif

#include "state_machine.hpp"
#include "xml/serialization.hpp"
#include "lexical_analysis/token.hpp"

#ifdef _DEBUG
#define assert(expr)                           \
  if (!(expr)) {                               \
    throw std::exception("assertion failed!"); \
  }
#endif