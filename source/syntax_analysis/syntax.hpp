// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif  //__cplusplus

#include <string>
#include <memory>
#include <vector>

enum class SyntaxKind {
  Unknown,

  Source,

  ExprConstNumber,
  ExprBinary,

  Statement,
  StatementBlock,
  StmReturn,

  DefVar,
  DefFunc,
  DefParam,
  DefStruct,
  DefClass,
  DefInterface,
};

constexpr auto is_expression(SyntaxKind kind) -> bool {
  switch (kind) {
    case SyntaxKind::ExprConstNumber:
    case SyntaxKind::ExprBinary: {
      return true;
    }
    default:
      return false;
  }
}

constexpr auto is_statement(SyntaxKind kind) -> bool {
  switch (kind) {
    case SyntaxKind::StmReturn:
      return true;
    default:
      return false;
  }
}

enum class Operator {
  Add,
  Subtract,
  Multiply,
  Divide,

  Equals,
  NotEquals,
  Or,
  And,

  LessThan,
  GreaterThan,
  LessThanEquals,
  GreaterThanEquals,

  BitOr,
  BitAnd,
  BitXor,

  ShiftLeft,
  ShiftRight
};

enum class Precedence {

  BitOr,
  BitXor,
  BitAnd,
  Equality,
  Relation,
  Shift,
  AddSub,
  MulDiv,
};

constexpr auto get_precedence(Operator op) -> Precedence {
  switch (op) {
    case Operator::Equals:
    case Operator::NotEquals: {
      return Precedence::Equality;
    }
    case Operator::LessThan:
    case Operator::GreaterThan:
    case Operator::LessThanEquals:
    case Operator::GreaterThanEquals: {
      return Precedence::Relation;
    }

    case Operator::ShiftLeft:
    case Operator::ShiftRight: {
      return Precedence::Shift;
    }
    case Operator::Add:
    case Operator::Subtract: {
      return Precedence::AddSub;
    }
    case Operator::Multiply:
    case Operator::Divide: {
      return Precedence::MulDiv;
    }

    default:
      throw std::exception();
  }
}

class SyntaxNode {
 private:
  const SyntaxKind kind_;

 protected:
  explicit constexpr SyntaxNode(const SyntaxKind kind) : kind_{kind} {}

 public:
  virtual ~SyntaxNode() = default;

  constexpr auto kind() const noexcept { return kind_; }
};

template <typename T>
concept IsSyntaxNode = requires(T) { std::is_base_of_v<SyntaxNode, T>; };

class ExpressionNode : public SyntaxNode {
 protected:
  explicit constexpr ExpressionNode(const SyntaxKind kind) : SyntaxNode{kind} {}
};

class Operation : public ExpressionNode {
  Operator op_;

 public:
  constexpr Operation(SyntaxKind kind, Operator op)
      : ExpressionNode{kind}, op_{op} {}

  auto op() const { return op_; }
};

class BinaryExpression final : public Operation {
 public:
  using Expression = std::shared_ptr<ExpressionNode>;

 private:
  Expression lhs_;
  Expression rhs_;

 public:
  constexpr BinaryExpression(Operator op)
      : Operation{SyntaxKind::ExprBinary, op} {}

  auto set_lhs(const Expression& lhs) -> void { lhs_ = lhs; }

  auto set_rhs(const Expression& rhs) -> void { rhs_ = rhs; }
};

class ConstantExpressionNode : public ExpressionNode {
 public:
  using ValueType = std::shared_ptr<const std::string>;

 private:
  ValueType value_;

 protected:
  explicit ConstantExpressionNode(const SyntaxKind kind, const ValueType& value)
      : ExpressionNode{kind}, value_{value} {}
};

class ConstantNumberExpressionNode : public ConstantExpressionNode {
 public:
  explicit ConstantNumberExpressionNode(const ValueType& value)
      : ConstantExpressionNode{SyntaxKind::ExprConstNumber, value} {}
};

class Definition : public SyntaxNode {
  String name_;

 protected:
  Definition(SyntaxKind kind, const String& name = nullptr)
      : SyntaxNode(kind), name_{name} {}

 public:
  auto set_name(const String& name) noexcept -> void { name_ = name; }
};

class VarDefinition : public Definition {
 public:
  using Assignment = std::shared_ptr<ExpressionNode>;

 private:
  String type_name_;
  Assignment assignment_;

 public:
  explicit VarDefinition(const String& name = nullptr,
                         const String& type_name = nullptr,
                         const Assignment& assign = nullptr)
      : Definition(SyntaxKind::DefVar, name),
        type_name_{type_name},
        assignment_{assign} {}

 public:
  auto set_type_name(const String& type_name) noexcept -> void {
    type_name_ = type_name;
  }

  auto set_assignment(const Assignment& assignment) noexcept -> void {
    assignment_ = assignment;
  }
};

class FuncParameter final : public Definition {
 private:
  String type_name_;

 public:
  explicit FuncParameter(const String& name = nullptr)
      : Definition(SyntaxKind::DefParam, name) {}

  auto set_type_name(const String& type_name) noexcept -> void {
    type_name_ = type_name;
  }
};

class Statement : public SyntaxNode {
 public:
  constexpr explicit Statement(SyntaxKind kind) : SyntaxNode{kind} {}
};

class ReturnStatement final : public Statement {
  std::shared_ptr<ExpressionNode> _expression;

 public:
  constexpr ReturnStatement() : Statement{SyntaxKind::StmReturn} {}

  auto set_expr(const std::shared_ptr<ExpressionNode>& expr) {
    _expression = expr;
  }
};

class VarStatement final : public Statement {};

class BodyStatement : public Statement {};

class IfStatement final : public BodyStatement {};

class ElifStatement final : public BodyStatement {};

class ElseStatement final : public BodyStatement {};

class LoopStatement final : public BodyStatement {};

class WhileStatement final : public BodyStatement {};

class ForStatement final : public BodyStatement {};

class ForeachStatement final : public BodyStatement {};

class StatementBlock final : public SyntaxNode {
 public:
  using Statement = std::shared_ptr<Statement>;
  using StatementCollection = std::vector<Statement>;

 private:
  StatementCollection statements_;

 public:
  explicit StatementBlock() : SyntaxNode(SyntaxKind::StatementBlock) {}
  auto push_statement(const Statement& statement) -> void {
    statements_.emplace_back(statement);
  }
};

class FuncDefinition final : public Definition {
 public:
  using Parameter = std::shared_ptr<FuncParameter>;
  using ParameterCollection = std::vector<Parameter>;

 private:
  ParameterCollection parameters_;
  String return_;

 public:
  explicit FuncDefinition() : Definition(SyntaxKind::DefFunc) {}

  auto set_return_type(const String& ret_type) -> void { return_ = ret_type; }

  auto push_parameter(const Parameter& param) -> void {
    parameters_.emplace_back(param);
  }
};

class SourceNode final : public SyntaxNode {
  using Node = std::shared_ptr<SyntaxNode>;
  using NodeArray = std::vector<Node>;

  NodeArray nodes_;

 public:
  constexpr SourceNode() : SyntaxNode(SyntaxKind::Source) {}

  auto push_node(const Node& node) { nodes_.emplace_back(node); }
};


constexpr auto get_binary_op(LexicalKind kind) -> Operator {
  switch (kind) {
    case LexicalKind::SymbolPlus:
      return Operator::Add;
    case LexicalKind::SymbolMinus:
      return Operator::Subtract;
    case LexicalKind::SymbolStar:
      return Operator::Multiply;
    case LexicalKind::SymbolSlash:
      return Operator::Divide;

    case LexicalKind::SymbolBoolEquals:
      return Operator::Equals;
    case LexicalKind::SymbolBoolNotEquals:
      return Operator::NotEquals;
    case LexicalKind::SymbolBoolOr:
      return Operator::Or;
    case LexicalKind::SymbolBoolAnd:
      return Operator::And;

    case LexicalKind::SymbolAngleOpen:
      return Operator::LessThan;
    case LexicalKind::SymbolAngleClose:
      return Operator::GreaterThan;
    case LexicalKind::SymbolLessThanEqual:
      return Operator::LessThanEquals;
    case LexicalKind::SymbolGreaterThanEqual:
      return Operator::GreaterThanEquals;

    case LexicalKind::SymbolBitOr:
      return Operator::BitOr;
    case LexicalKind::SymbolBitAnd:
      return Operator::BitAnd;
    case LexicalKind::SymbolBitXor:
      return Operator::BitXor;

    case LexicalKind::SymbolShiftLeft:
      return Operator::ShiftLeft;
    case LexicalKind::SymbolShiftRight:
      return Operator::ShiftRight;

    default:
      throw std::exception();
  }
}