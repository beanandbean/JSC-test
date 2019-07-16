#ifndef jsast_ast_hpp
#define jsast_ast_hpp

#include <optional>
#include <string>

#include "ast_node.hpp"
#include "node_vector.hpp"
#include "specs.hpp"

namespace jsast {

// Useful ast-related tags
enum class unary_op_location { prefix, suffix };
enum class binary_operand_location { left, right };

namespace ast {

struct base {
  inline base() noexcept {}
};

struct super : base {
  using base::base;
};

struct statement : base {
 protected:
  using base::base;
};

struct empty_statement : statement {
  using statement::statement;
};

struct expression_statement : statement {
  node expression;

  inline expression_statement(node&& _expression)
      : expression{std::move(_expression)} {}
};

struct labeled_statement : statement {
  node label;
  node body;

  inline labeled_statement(node&& _label, node&& _body)
      : label{std::move(_label)}, body{std::move(_body)} {}
};

struct break_statement : statement {
  std::optional<node> label;

  inline break_statement() noexcept {}
  inline break_statement(node&& _label) : label{std::move(_label)} {}
};

struct continue_statement : statement {
  std::optional<node> label;

  inline continue_statement() noexcept {}
  inline continue_statement(node&& _label) : label{std::move(_label)} {}
};

struct with_statement : statement {
  node object;
  node body;

  inline with_statement(node&& _object, node&& _body)
      : object{std::move(_object)}, body{std::move(_body)} {}
};

struct return_statement : statement {
  std::optional<node> argument;

  inline return_statement() noexcept {}
  inline return_statement(node&& _argument) : argument{std::move(_argument)} {}
};

struct throw_statement : statement {
  node argument;

  inline throw_statement(node&& _argument) : argument{std::move(_argument)} {}
};

struct expression : base {
  using base::base;
};

struct this_expression : expression {
  using expression::expression;
};

struct unary_expression : expression {
  unary_op op;
  node argument;

  inline unary_expression(unary_op _op, node&& _argument)
      : op{_op}, argument{std::move(_argument)} {}
};

struct binary_expression : expression {
  node left;
  binary_op op;
  node right;

  inline binary_expression(node&& _left, binary_op _op, node&& _right)
      : left{std::move(_left)}, op{_op}, right{std::move(_right)} {}
};

struct assignment_expression : expression {
  node left;
  assignment_op op;
  node right;

  inline assignment_expression(node&& _left, node&& _right)
      : assignment_expression{std::move(_left), assignment_op::standard,
                              std::move(_right)} {}
  inline assignment_expression(node&& _left, assignment_op _op, node&& _right)
      : left{std::move(_left)}, op{_op}, right{std::move(_right)} {}
};

struct update_expression : expression {
  update_op op;
  node argument;
  unary_op_location loc;

  inline update_expression(update_op _op, node&& _argument,
                           unary_op_location _loc)
      : op{_op}, argument{std::move(_argument)}, loc{_loc} {}
};

struct logical_expression : expression {
  node left;
  logical_op op;
  node right;

  inline logical_expression(node&& _left, logical_op _op, node&& _right)
      : left{std::move(_left)}, op{_op}, right{std::move(_right)} {}
};

struct conditional_expression : expression {
  node test;
  node consequent;
  node alternate;

  inline conditional_expression(node&& _test, node&& _consequent,
                                node&& _alternate)
      : test{std::move(_test)},
        consequent{std::move(_consequent)},
        alternate{std::move(_alternate)} {}
};

struct base_call_expression : expression {
  node callee;
  node_vector arguments;

  inline base_call_expression(node&& _callee, node_vector&& _arguments)
      : callee{std::move(_callee)}, arguments{std::move(_arguments)} {}
};

struct call_expression : base_call_expression {
  using base_call_expression::base_call_expression;
};

struct new_expression : base_call_expression {
  using base_call_expression::base_call_expression;
};

struct member_expression : expression {
  node object;
  std::string property;

  inline member_expression(node&& _object, std::string _property)
      : object{std::move(_object)}, property{_property} {}
};

struct computed_member_expression : expression {
  node object;
  node property;

  inline computed_member_expression(node&& _object, node&& _property)
      : object{std::move(_object)}, property{std::move(_property)} {}
};

struct yield_expression : expression {
  std::optional<node> argument;

  inline yield_expression() noexcept {}
  inline yield_expression(node&& _argument) : argument{std::move(_argument)} {}
};

struct delegate_yield_expression : expression {
  node argument;

  inline delegate_yield_expression(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct await_expression : expression {
  node argument;

  inline await_expression(node&& _argument) : argument{std::move(_argument)} {}
};

struct pattern : base {
  using base::base;
};

struct identifier : pattern {
  std::string name;

  inline identifier(std::string _name) : name{_name} {}
};

struct assignment_pattern : pattern {
  node left;
  node right;

  inline assignment_pattern(node&& _left, node&& _right)
      : left{std::move(_left)}, right{std::move(_right)} {}
};

struct rest_element : pattern {
  node argument;

  inline rest_element(node&& _argument) : argument{std::move(_argument)} {}
};

struct spread_element : base {
  node argument;

  inline spread_element(node&& _argument) : argument{std::move(_argument)} {}
};

struct literal : expression {
  using expression::expression;
};

struct null_literal : literal {
  using literal::literal;
};

struct bool_literal : literal {
  bool value;

  inline bool_literal(bool _value) noexcept : value{_value} {}
};

struct number_literal : literal {
  std::string number;

  template <typename number_type>
  inline number_literal(number_type _number)
      : number{std::to_string(_number)} {}
};

struct string_literal : literal {
  std::string string;

  inline string_literal(std::string _string) : string{_string} {}
};

struct raw_literal : literal {
  std::string raw;

  inline raw_literal(std::string _raw) : raw{_raw} {}
};

}  // namespace ast

}  // namespace jsast

#endif  // jsast_ast_hpp