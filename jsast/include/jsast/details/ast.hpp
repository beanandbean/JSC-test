#ifndef jsast_ast_hpp
#define jsast_ast_hpp

#include <optional>
#include <string>

#include "ast_node.hpp"
#include "specs.hpp"
#include "utils.hpp"

namespace jsast::ast {

struct base {
  explicit inline base() noexcept {}
};

struct program : base {
  utils::move_vector<node> body;

  explicit inline program(utils::move_vector<node>&& _body)
      : body{std::move(_body)} {}
};

struct super : base {
  using base::base;
};

struct member_identifier : base {
  std::string name;

  explicit inline member_identifier(std::string _name) : name{_name} {}
};

struct property : base {
  node key;
  node value;

  explicit inline property(node&& _key, node&& _value)
      : key{std::move(_key)}, value{std::move(_value)} {}
};

struct switch_case : base {
  std::optional<node> test;
  utils::move_vector<node> consequent;

  explicit inline switch_case(utils::move_vector<node>&& _consequent)
      : switch_case{std::nullopt, std::move(_consequent)} {}
  explicit inline switch_case(std::optional<node>&& _test,
                              utils::move_vector<node>&& _consequent)
      : test{std::move(_test)}, consequent{std::move(_consequent)} {}
};

struct catch_clause : base {
  std::optional<node> pattern;
  node body;

  explicit inline catch_clause(node&& _body)
      : catch_clause{std::nullopt, std::move(_body)} {}
  explicit inline catch_clause(std::optional<node>&& _pattern, node&& _body)
      : pattern{std::move(_pattern)}, body{std::move(_body)} {}
};

struct variable_declarator : base {
  node id;
  std::optional<node> init;

  explicit inline variable_declarator(
      node&& _id, std::optional<node>&& _init = std::nullopt)
      : id{std::move(_id)}, init{std::move(_init)} {}
};

struct template_element : base {
  std::string value;

  explicit inline template_element(std::string _value) : value{_value} {}
};

struct statement : base {
  using base::base;
};

struct empty_statement : statement {
  using statement::statement;
};

struct block_statement : statement {
  utils::move_vector<node> body;

  explicit inline block_statement(utils::move_vector<node>&& _body)
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
  std::string label;
  node body;

  explicit inline labeled_statement(std::string _label, node&& _body)
      : label{_label}, body{std::move(_body)} {}
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

struct switch_statement : statement {
  node discriminant;
  utils::move_vector<node> cases;

  explicit inline switch_statement(node&& _discriminant,
                                   utils::move_vector<node>&& _cases)
      : discriminant{std::move(_discriminant)}, cases{std::move(_cases)} {}
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

struct try_statement : statement {
  node block;
  std::optional<node> handler;
  std::optional<node> finalizer;

  explicit inline try_statement(node&& _block,
                                std::optional<node>&& _handler = std::nullopt,
                                std::optional<node>&& _finalizer = std::nullopt)
      : block{std::move(_block)},
        handler{std::move(_handler)},
        finalizer{std::move(_finalizer)} {}
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

struct for_statement : statement {
  std::optional<node> init;
  std::optional<node> test;
  std::optional<node> update;
  node body;

  explicit inline for_statement(std::optional<node>&& _init,
                                std::optional<node>&& _test,
                                std::optional<node>&& _update, node&& _body)
      : init{std::move(_init)},
        test{std::move(_test)},
        update{std::move(_update)},
        body{std::move(_body)} {}
};

struct for_in_statement : statement {
  node left;
  node right;
  node body;
  bool await;

  explicit inline for_in_statement(node&& _left, node&& _right, node&& _body,
                                   bool _await = false)
      : left{std::move(_left)},
        right{std::move(_right)},
        body{std::move(_body)},
        await{_await} {}
};

struct for_of_statement : statement {
  node left;
  node right;
  node body;
  bool await;

  explicit inline for_of_statement(node&& _left, node&& _right, node&& _body,
                                   bool _await = false)
      : left{std::move(_left)},
        right{std::move(_right)},
        body{std::move(_body)},
        await{_await} {}
};

struct debugger_statement : statement {
  using statement::statement;
};

struct declaration : statement {
  using statement::statement;
};

struct variable_declaration : declaration {
  utils::move_vector<node> declarations;
  variable_declaration_type kind;

  explicit inline variable_declaration(utils::move_vector<node>&& _declarations,
                                       variable_declaration_type _kind)
      : declarations{std::move(_declarations)}, kind{_kind} {}
};

struct function_declaration : declaration {
  std::string id;
  utils::move_vector<node> params;
  node body;
  bool async;
  bool generator;

  explicit inline function_declaration(std::string _id,
                                       utils::move_vector<node>&& _params,
                                       node&& _body, bool _async = false,
                                       bool _generator = false)
      : id{_id},
        params{std::move(_params)},
        body{std::move(_body)},
        async{_async},
        generator{_generator} {}
};

struct expression : base {
  using base::base;
};

struct this_expression : expression {
  using expression::expression;
};

struct array_expression : expression {
  utils::move_vector<std::optional<node>> elements;

  explicit inline array_expression(
      utils::move_vector<std::optional<node>>&& _elements)
      : elements{std::move(_elements)} {}
};

struct object_expression : expression {
  utils::move_vector<node> properties;

  explicit inline object_expression(utils::move_vector<node>&& _properties)
      : properties{std::move(_properties)} {}
};

struct function_expression : expression {
  std::optional<std::string> id;
  utils::move_vector<node> params;
  node body;
  bool async;
  bool generator;

  explicit inline function_expression(utils::move_vector<node>&& _params,
                                      node&& _body, bool _async = false,
                                      bool _generator = false)
      : function_expression{std::nullopt, std::move(_params), std::move(_body),
                            _async, _generator} {}
  explicit inline function_expression(std::optional<std::string> _id,
                                      utils::move_vector<node>&& _params,
                                      node&& _body, bool _async = false,
                                      bool _generator = false)
      : id{_id},
        params{std::move(_params)},
        body{std::move(_body)},
        async{_async},
        generator{_generator} {}
};

struct arrow_function_expression : expression {
  utils::move_vector<node> params;
  node body;
  bool async;

  explicit inline arrow_function_expression(utils::move_vector<node>&& _params,
                                            node&& _body, bool _async = false)
      : params{std::move(_params)}, body{std::move(_body)}, async{_async} {}
};

struct sequence_expression : expression {
  utils::move_vector<node> expressions;

  explicit inline sequence_expression(utils::move_vector<node>&& _expressions)
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
  utils::move_vector<node> arguments;

  explicit inline base_call_expression(node&& _callee,
                                       utils::move_vector<node>&& _arguments)
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
  node property;

  explicit inline member_expression(node&& _object, node&& _property)
      : object{std::move(_object)}, property{std::move(_property)} {}
};

struct yield_expression : expression {
  std::optional<node> argument;
  bool delegate;

  explicit inline yield_expression(
      std::optional<node>&& _argument = std::nullopt, bool _delegate = false)
      : argument{std::move(_argument)}, delegate{_delegate} {}
};

struct await_expression : expression {
  node argument;

  explicit inline await_expression(node&& _argument)
      : argument{std::move(_argument)} {}
};

struct template_literal : expression {
  utils::move_vector<node> quasis;

  explicit inline template_literal(utils::move_vector<node>&& _quasis)
      : quasis{std::move(_quasis)} {}
};

struct tagged_template_expression : expression {
  node tag;
  node quasi;

  explicit inline tagged_template_expression(node&& _tag, node&& _quasi)
      : tag{std::move(_tag)}, quasi{std::move(_quasi)} {}
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
  utils::move_vector<std::optional<node>> elements;

  explicit inline array_pattern(
      utils::move_vector<std::optional<node>>&& _elements)
      : elements{std::move(_elements)} {}
};

struct object_pattern : pattern {
  utils::move_vector<node> properties;

  explicit inline object_pattern(utils::move_vector<node>&& _properties)
      : properties{std::move(_properties)} {}
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

struct reg_exp_literal : literal {
  std::string pattern;
  std::string flags;

  explicit inline reg_exp_literal(std::string _pattern, std::string _flags = "")
      : pattern{_pattern}, flags{_flags} {}
};

struct raw_literal : literal {
  std::string raw;

  explicit inline raw_literal(std::string _raw) : raw{_raw} {}
};

}  // namespace jsast::ast

#endif  // jsast_ast_hpp