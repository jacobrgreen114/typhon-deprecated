// Copyright (c) 2023 Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif

#include "project_tree.hpp"

class BaseSymbol {
  std::string name_;

 public:
};

class IncludeSymbol : public BaseSymbol {
 public:
  using Pointer = std::unique_ptr<IncludeSymbol>;

 private:
};

class CTypeSymbol : public BaseSymbol {
 public:
  using Pointer = std::unique_ptr<CTypeSymbol>;

 private:
};

class VariableSymbol : public BaseSymbol {
 public:
  using Pointer = std::unique_ptr<VariableSymbol>;

 private:
  std::string type_;
};

class StructSymbol : public BaseSymbol {
 public:
  using Pointer = std::unique_ptr<StructSymbol>;

 private:
  std::string type_;
};

class SymbolSpace : public BaseSymbol {
 public:
  using Pointer = std::unique_ptr<SymbolSpace>;

 private:
  std::vector<IncludeSymbol::Pointer> includes_;
  std::vector<CTypeSymbol::Pointer> ctypes_;
  std::vector<VariableSymbol::Pointer> variables_;
  std::vector<StructSymbol::Pointer> structs_;
};

class SymbolTable {
 public:
  using Pointer = std::unique_ptr<SymbolTable>;

 private:
  std::string project_name_;

 public:
  static auto from_project_tree() -> Pointer;
};
