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
  explicit inline base() noexcept {}
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

  explicit inline expression_statement(node&& _expression)
      : expression{std::move(_expression)} {}
};

struct labeled_statement : statement {
  node label;
  node body;

  explicit inline labeled_statement(node&& _label, node&& _body)
      : label{std::move(_label)}, body{std::move(_body)} {}
};

struct break_statement : statement {
  std::optional<node> label;

  explicit inline break_statement() noexcept {}
  explicit inline break_statement(node&& _label) : label{std::move(_label)} {}
};

struct continue_statement : statement {
  std::optional<node> label;

  explicit inline continue_statement() noexcept {}
  explicit inline continue_statement(node&& _label)
      : label{std::move(_label)} {}
};

struct with_statement : statement {
  node object;
  node body;

  explicit inline with_statement(node&& _object, node&& _body)
      : object{std::move(_object)}, body{std::move(_body)} {}
};

struct return_statement : statement {
  std::optional<node> argument;

  explicit inline return_statement() noexcept {}
  explicit inline return_statement(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct throw_statement : statement {
  node argument;

  explicit inline throw_statement(node&& _argument)
      : argument{std::move(_argument)} {}
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

  explicit inline unary_expression(unary_op _op, node&& _argument)
      : op{_op}, argument{std::move(_argument)} {}
};

struct binary_expression : expression {
  node left;
  binary_op op;
  node right;

  explicit inline binary_expression(node&& _left, binary_op _op, node&& _right)
      : left{std::move(_left)}, op{_op}, right{std::move(_right)} {}
};

struct assignment_expression : expression {
  node left;
  assignment_op op;
  node right;

  explicit inline assignment_expression(node&& _left, node&& _right)
      : assignment_expression{std::move(_left), assignment_op::standard,
                              std::move(_right)} {}
  explicit inline assignment_expression(node&& _left, assignment_op _op,
                                        node&& _right)
      : left{std::move(_left)}, op{_op}, right{std::move(_right)} {}
};

struct update_expression : expression {
  update_op op;
  node argument;
  unary_op_location loc;

  explicit inline update_expression(update_op _op, node&& _argument,
                                    unary_op_location _loc)
      : op{_op}, argument{std::move(_argument)}, loc{_loc} {}
};

struct logical_expression : expression {
  node left;
  logical_op op;
  node right;

  explicit inline logical_expression(node&& _left, logical_op _op,
                                     node&& _right)
      : left{std::move(_left)}, op{_op}, right{std::move(_right)} {}
};

struct conditional_expression : expression {
  node test;
  node consequent;
  node alternate;

  explicit inline conditional_expression(node&& _test, node&& _consequent,
                                         node&& _alternate)
      : test{std::move(_test)},
        consequent{std::move(_consequent)},
        alternate{std::move(_alternate)} {}
};

struct base_call_expression : expression {
  node callee;
  node_vector arguments;

  explicit inline base_call_expression(node&& _callee, node_vector&& _arguments)
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

  explicit inline member_expression(node&& _object, std::string _property)
      : object{std::move(_object)}, property{_property} {}
};

struct computed_member_expression : expression {
  node object;
  node property;

  explicit inline computed_member_expression(node&& _object, node&& _property)
      : object{std::move(_object)}, property{std::move(_property)} {}
};

struct yield_expression : expression {
  std::optional<node> argument;

  explicit inline yield_expression() noexcept {}
  explicit inline yield_expression(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct delegate_yield_expression : expression {
  node argument;

  explicit inline delegate_yield_expression(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct await_expression : expression {
  node argument;

  explicit inline await_expression(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct meta_property : expression {
  std::string meta;
  std::string property;

  explicit inline meta_property(std::string _meta, std::string _property)
      : meta{_meta}, property{_property} {}
};

struct pattern : base {
  using base::base;
};

struct identifier : pattern {
  std::string name;

  explicit inline identifier(std::string _name) : name{_name} {}
};

struct assignment_pattern : pattern {
  node left;
  node right;

  explicit inline assignment_pattern(node&& _left, node&& _right)
      : left{std::move(_left)}, right{std::move(_right)} {}
};

struct rest_element : pattern {
  node argument;

  explicit inline rest_element(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct spread_element : base {
  node argument;

  explicit inline spread_element(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct literal : expression {
  using expression::expression;
};

struct null_literal : literal {
  using literal::literal;
};

struct bool_literal : literal {
  bool value;

  explicit inline bool_literal(bool _value) noexcept : value{_value} {}
};

struct number_literal : literal {
  std::string number;

  template <typename number_type>
  explicit inline number_literal(number_type _number)
      : number{std::to_string(_number)} {}
};

struct string_literal : literal {
  std::string string;

  explicit inline string_literal(std::string _string) : string{_string} {}
};

struct raw_literal : literal {
  std::string raw;

  explicit inline raw_literal(std::string _raw) : raw{_raw} {}
};

}  // namespace ast

}  // namespace jsast

#endif  // jsast_ast_hpp