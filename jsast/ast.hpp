#ifndef jsast_ast_hpp
#define jsast_ast_hpp

#include <optional>
#include <string>

#include "../utils/move_vector.hpp"
#include "ast_node.hpp"
#include "specs.hpp"

namespace jsast {

namespace ast {

struct base {
  explicit inline base() noexcept {}
};

struct super : base {
  using base::base;
};

struct variable_declarator : base {
  node id;
  std::optional<node> init;

  explicit inline variable_declarator(
      node&& _id, std::optional<node>&& _init = std::nullopt)
      : id{std::move(_id)}, init{std::move(_init)} {}
};

struct statement : base {
  using base::base;
};

struct empty_statement : statement {
  using statement::statement;
};

struct block_statement : statement {
  move_vector<node> body;

  explicit inline block_statement(move_vector<node>&& _body)
      : body{std::move(_body)} {}
};

struct expression_statement : statement {
  node expression;

  explicit inline expression_statement(node&& _expression)
      : expression{std::move(_expression)} {}
};

struct if_statement : statement {
  node test;
  node consequent;
  std::optional<node> alternate;

  explicit inline if_statement(node&& _test, node&& _consequent,
                               std::optional<node>&& _alternate = std::nullopt)
      : test{std::move(_test)},
        consequent{std::move(_consequent)},
        alternate{std::move(_alternate)} {}
};

struct labeled_statement : statement {
  node label;
  node body;

  explicit inline labeled_statement(node&& _label, node&& _body)
      : label{std::move(_label)}, body{std::move(_body)} {}
};

struct break_statement : statement {
  std::optional<node> label;

  explicit inline break_statement(std::optional<node>&& _label = std::nullopt)
      : label{std::move(_label)} {}
};

struct continue_statement : statement {
  std::optional<node> label;

  explicit inline continue_statement(
      std::optional<node>&& _label = std::nullopt)
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

  explicit inline return_statement(
      std::optional<node>&& _argument = std::nullopt)
      : argument{std::move(_argument)} {}
};

struct throw_statement : statement {
  node argument;

  explicit inline throw_statement(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct while_statement : statement {
  node test;
  node body;

  explicit inline while_statement(node&& _test, node&& _body)
      : test{std::move(_test)}, body{std::move(_body)} {}
};

struct do_while_statement : statement {
  node test;
  node body;

  explicit inline do_while_statement(node&& _test, node&& _body)
      : test{std::move(_test)}, body{std::move(_body)} {}
};

struct debugger_statement : statement {
  using statement::statement;
};

struct declaration : statement {
  using statement::statement;
};

struct variable_declaration : declaration {
  move_vector<node> declarations;
  variable_declaration_type kind;

  explicit inline variable_declaration(move_vector<node>&& _declarations,
                                       variable_declaration_type _kind)
      : declarations{std::move(_declarations)}, kind{_kind} {}
};

struct expression : base {
  using base::base;
};

struct this_expression : expression {
  using expression::expression;
};

struct array_expression : expression {
  move_vector<std::optional<node>> elements;

  explicit inline array_expression(move_vector<std::optional<node>>&& _elements)
      : elements{std::move(_elements)} {}
};

struct sequence_expression : expression {
  move_vector<node> expressions;

  explicit inline sequence_expression(move_vector<node>&& _expressions)
      : expressions{std::move(_expressions)} {}
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
  move_vector<node> arguments;

  explicit inline base_call_expression(node&& _callee,
                                       move_vector<node>&& _arguments)
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

  explicit inline yield_expression(
      std::optional<node>&& _argument = std::nullopt)
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

struct array_pattern : pattern {
  move_vector<std::optional<node>> elements;

  explicit inline array_pattern(move_vector<std::optional<node>>&& _elements)
      : elements{std::move(_elements)} {}
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