#ifndef jsast_ast_hpp
#define jsast_ast_hpp

#include <string>

#include "ast_node.hpp"
#include "specs.hpp"

namespace jsast {

// Reference only
enum class ast_node_type {
  unexpected_node /* should never occur! */,
  program,
  super,
  block_statement,
  class_body,
  empty_statement,
  expression_statement,
  if_statement,
  labeled_statement,
  break_statement,
  continue_statement,
  with_statement,
  switch_statement,
  return_statement,
  throw_statement,
  try_statement,
  while_statement,
  do_while_statement,
  for_statement,
  for_in_statement,
  for_of_statement,
  debugger_statement,
  function_declaration,
  function_expression,
  variable_declaration,
  variable_declarator,
  class_declaration,
  import_declaration,
  export_default_declaration,
  export_named_declaration,
  export_all_declaration,
  method_definition,
  class_expression,
  arrow_function_expression,
  this_expression,
  rest_element,
  spread_element,
  yield_expression,
  await_expression,
  template_literal,
  tagged_template_expression,
  array_expression,
  array_pattern,
  object_expression,
  property,
  object_pattern,
  sequence_expression,
  unary_expression,
  update_expression,
  assignment_expression,
  assignment_pattern,
  binary_expression,
  logical_expression,
  conditional_expression,
  new_expression,
  call_expression,
  member_expression,
  meta_property,
  identifier,
  null_literal,
  bool_literal,
  number_literal,
  string_literal,
  reg_exp_literal,
  raw_literal,
};

// Useful ast-related tags
enum class unary_op_location { prefix, suffix };
enum class binary_operand_location { left, right };

namespace ast {

struct base {
  virtual ~base() noexcept {}

 protected:
  inline base() noexcept {}
};

struct super : base {
  using base::base;
};

struct expression : base {
 protected:
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

struct pattern : base {
 protected:
  inline pattern() noexcept {}
};

struct identifier : pattern {
  std::string name;

  inline identifier(std::string _name) : name{_name} {}
};

struct literal : expression {
 protected:
  inline literal() noexcept {}
};

struct null_literal : literal {
  inline null_literal() noexcept {}
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