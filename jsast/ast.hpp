#ifndef jsast_ast_hpp
#define jsast_ast_hpp

#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>

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

struct generator;

namespace ast {

struct base;

struct node {
  friend generator;

  struct impl_base {
    friend node;

    virtual ~impl_base() noexcept {}
    virtual const base& get() const = 0;
    virtual std::type_index type() const = 0;

   private:
    virtual void write_to(generator& g) const = 0;
  };

  template <typename node_type,
            typename = std::enable_if<std::is_base_of_v<base, node_type>>>
  struct impl : impl_base {
    inline impl(node_type&& node) : _node{std::move(node)} {}

    const base& get() const override { return _node; }
    std::type_index type() const override { return typeid(node_type); }
    void write_to(generator& g) const override;

   private:
    node_type _node;
  };

  template <typename node_type>
  inline node(node_type&& node)
      : _impl{
            std::make_unique<impl<node_type>>(std::forward<node_type>(node))} {}

  inline const base& get() const { return _impl->get(); }
  inline std::type_index type() const { return _impl->type(); }

 private:
  std::unique_ptr<impl_base> _impl;

  inline void write_to(generator& g) const { _impl->write_to(g); }
};

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

template <typename node_type>
inline bool node_is(const ast::node& node) {
  return node.type() == std::type_index{typeid(node_type)};
}

}  // namespace jsast

#endif  // jsast_ast_hpp