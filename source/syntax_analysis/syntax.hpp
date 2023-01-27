// Copyright (c) 2023. Jacob R. Green
// All Rights Reserved.

#pragma once

#ifndef __cplusplus
#error
#endif  //__cplusplus

#include <string>
#include <memory>
#include <vector>

#include "../xml/serialization.hpp"

enum class SyntaxKind {
  Unknown,

  Source,

  ExprNumber,
  //ExprString,
  ExprUnary,
  ExprBinary,
  // ExprTernary,

  StmExpr,
  StmReturn,

  Block,

  DefVar,
  DefFunc,
  DefParam,
  DefStruct,
  DefClass,
  DefInterface,
};

enum class Precedence {
  Assignment,
  BoolOr,
  BoolAnd,
  BitOr,
  BitXor,
  BitAnd,
  Equality,
  Relation,
  Shift,
  AddSub,
  MulDiv,
  Prefix,
  Postfix,
};

enum class OperatorType { Unary = 1, Binary = 2, Ternary = 3 };

constexpr auto operator_type_offset = 30;
constexpr auto operator_prec_offset = 24;

consteval auto operator_value(OperatorType type, Precedence precedence,
                              uint32_t base_value = 0) -> uint32_t {
  return (static_cast<uint32_t>(type) << operator_type_offset) |
         (static_cast<uint32_t>(precedence) << operator_prec_offset) |
         base_value;
}

enum class Operator : uint32_t {
  // Binary
  Add      = operator_value(OperatorType::Binary, Precedence::AddSub, 0),
  Subtract = operator_value(OperatorType::Binary, Precedence::AddSub, 1),
  Multiply = operator_value(OperatorType::Binary, Precedence::MulDiv, 0),
  Divide   = operator_value(OperatorType::Binary, Precedence::MulDiv, 1),

  Equals    = operator_value(OperatorType::Binary, Precedence::Equality, 0),
  NotEquals = operator_value(OperatorType::Binary, Precedence::Equality, 1),
  Or        = operator_value(OperatorType::Binary, Precedence::BoolOr, 0),
  And       = operator_value(OperatorType::Binary, Precedence::BoolAnd, 0),

  LessThan    = operator_value(OperatorType::Binary, Precedence::Relation, 0),
  GreaterThan = operator_value(OperatorType::Binary, Precedence::Relation, 1),
  LessThanEquals =
      operator_value(OperatorType::Binary, Precedence::Relation, 2),
  GreaterThanEquals =
      operator_value(OperatorType::Binary, Precedence::Relation, 3),

  BitOr  = operator_value(OperatorType::Binary, Precedence::BitOr, 0),
  BitXor = operator_value(OperatorType::Binary, Precedence::BitXor, 0),
  BitAnd = operator_value(OperatorType::Binary, Precedence::BitAnd, 0),

  ShiftLeft  = operator_value(OperatorType::Binary, Precedence::Shift, 0),
  ShiftRight = operator_value(OperatorType::Binary, Precedence::Shift, 1),

  // Unary
  BoolNot = operator_value(OperatorType::Unary, Precedence::Prefix, 0),
  BitNot  = operator_value(OperatorType::Unary, Precedence::Prefix, 1),

  Positive = operator_value(OperatorType::Unary, Precedence::Prefix, 2),
  Negative = operator_value(OperatorType::Unary, Precedence::Prefix, 3),

  PreInc = operator_value(OperatorType::Unary, Precedence::Prefix, 4),
  PreDec = operator_value(OperatorType::Unary, Precedence::Prefix, 5),

  PostInc = operator_value(OperatorType::Unary, Precedence::Postfix, 0),
  PostDec = operator_value(OperatorType::Unary, Precedence::Postfix, 1),
};

auto to_string(SyntaxKind kind) -> const char*;

auto to_string(Operator op) -> const char*;

auto is_expression(SyntaxKind kind) -> bool;

auto is_statement(SyntaxKind kind) -> bool;

auto is_definition(SyntaxKind kind) -> bool;

auto get_unary_pre_op(LexicalKind kind) -> Operator;

auto get_binary_op(LexicalKind kind) -> Operator;

auto get_precedence(Operator op) -> Precedence;

class SyntaxNode
#ifdef TRACE
    : public xml::Serializable
#endif
{
 private:
  const SyntaxKind kind_;

 protected:
  explicit constexpr SyntaxNode(const SyntaxKind kind) : kind_{kind} {}

 public:
  virtual ~SyntaxNode() = default;

  constexpr auto kind() const noexcept { return kind_; }

 protected:
#ifdef TRACE
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

template <typename T>
concept IsSyntaxNode = requires(T) { std::is_base_of_v<SyntaxNode, T>; };

class ExpressionNode : public SyntaxNode {
 protected:
  explicit constexpr ExpressionNode(const SyntaxKind kind) : SyntaxNode{kind} {}
};

class Operation : public ExpressionNode {
 public:
  using Expression = std::shared_ptr<ExpressionNode>;

  Operator op_;

 public:
  constexpr Operation(SyntaxKind kind, Operator op)
      : ExpressionNode{kind}, op_{op} {}

  auto op() const { return op_; }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class UnaryExpression final : public Operation {
  Expression expr_;

 public:
  constexpr UnaryExpression(Operator op)
      : Operation{SyntaxKind::ExprUnary, op} {}

  auto set_expr(const Expression& expr) -> void { expr_ = expr; }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class BinaryExpression final : public Operation {
 private:
  Expression lhs_;
  Expression rhs_;

 public:
  constexpr BinaryExpression(Operator op)
      : Operation{SyntaxKind::ExprBinary, op} {}

  auto set_lhs(const Expression& lhs) -> void { lhs_ = lhs; }

  auto set_rhs(const Expression& rhs) -> void { rhs_ = rhs; }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class ConstantExpressionNode : public ExpressionNode {
 public:
  using ValueType = std::shared_ptr<const std::string>;

 private:
  ValueType value_;

 protected:
  explicit ConstantExpressionNode(const SyntaxKind kind, const ValueType& value)
      : ExpressionNode{kind}, value_{value} {}

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class ConstantNumberExpressionNode : public ConstantExpressionNode {
 public:
  explicit ConstantNumberExpressionNode(const ValueType& value)
      : ConstantExpressionNode{SyntaxKind::ExprNumber, value} {}
};

class Definition : public SyntaxNode {
  String name_;

 protected:
  Definition(SyntaxKind kind, const String& name = nullptr)
      : SyntaxNode(kind), name_{name} {}

 public:
  auto& name() const { return name_; }

  auto set_name(const String& name) noexcept -> void { name_ = name; }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class VarDefinition : public Definition {
 public:
  using Assignment = std::shared_ptr<ExpressionNode>;

 private:
  String type_name_;
  Assignment assignment_;

 public:
  explicit VarDefinition(const String& name       = nullptr,
                         const String& type_name  = nullptr,
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

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
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

class ExprStatement final : public Statement {
  std::shared_ptr<ExpressionNode> expr_;

 public:
  constexpr ExprStatement() : Statement{SyntaxKind::StmExpr} {}

  auto set_expr(const std::shared_ptr<ExpressionNode>& expr) { expr_ = expr; }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class ReturnStatement final : public Statement {
  std::shared_ptr<ExpressionNode> expr_;

 public:
  constexpr ReturnStatement() : Statement{SyntaxKind::StmReturn} {}

  auto set_expr(const std::shared_ptr<ExpressionNode>& expr) { expr_ = expr; }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
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
  using Statement           = std::shared_ptr<Statement>;
  using StatementCollection = std::vector<Statement>;

 private:
  StatementCollection statements_;

 public:
  explicit StatementBlock() : SyntaxNode(SyntaxKind::Block) {}
  auto push_statement(const Statement& statement) -> void {
    statements_.emplace_back(statement);
  }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class FuncDefinition final : public Definition {
 public:
  using Parameter           = std::shared_ptr<FuncParameter>;
  using ParameterCollection = std::vector<Parameter>;
  using Body                = std::shared_ptr<StatementBlock>;

 private:
  ParameterCollection parameters_;
  String return_;
  Body body_;

 public:
  explicit FuncDefinition() : Definition(SyntaxKind::DefFunc) {}

  void set_return_type(const String& ret_type) { return_ = ret_type; }

  void push_parameter(const Parameter& param) {
    parameters_.emplace_back(param);
  }

  auto set_body(const Body& body) { body_ = body; }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};

class SourceNode final : public SyntaxNode {
  using Node      = std::shared_ptr<SyntaxNode>;
  using NodeArray = std::vector<Node>;

  NodeArray nodes_;

 public:
  constexpr SourceNode() : SyntaxNode(SyntaxKind::Source) {}

  auto push_node(const Node& node) { nodes_.emplace_back(node); }

#ifdef TRACE
 protected:
  auto on_serialize(xml::SerializationContext& context) const -> void override;
#endif
};
