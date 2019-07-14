#ifndef jscodegen_ast_hpp
#define jscodegen_ast_hpp

#include <string>

#include "specs.hpp"

namespace jscodegen {

enum class unary_op_location { prefix, suffix };

namespace ast {

struct base {
 protected:
  inline base() noexcept {}
};

struct super : base {
  inline super() noexcept {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::super;
  }
};

struct expression : base {
 protected:
  inline expression() noexcept {}
};

struct this_expression : expression {
  inline this_expression() noexcept {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::this_expression;
  }
};

template <typename argument_type>
struct unary_expression : expression {
  unary_op op;
  argument_type argument;

  inline unary_expression(unary_op _op, argument_type&& _argument)
      : op{_op}, argument{std::forward<argument_type>(_argument)} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::unary_expression;
  }
};

template <typename left_type, typename right_type>
struct assignment_expression : expression {
  left_type left;
  assignment_op op;
  right_type right;

  inline assignment_expression(left_type&& _left, assignment_op _op, right_type&& _right)
      : left{std::forward<left_type>(_left)}, op{_op}, right{std::forward<right_type>(_right)} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::assignment_expression;
  }
};

template <typename argument_type>
struct update_expression : expression {
  update_op op;
  argument_type argument;
  unary_op_location loc;

  inline update_expression(update_op _op, argument_type&& _argument,
                           unary_op_location _loc)
      : op{_op}, argument{std::forward<argument_type>(_argument)}, loc{_loc} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::update_expression;
  }
};

struct pattern : base {
 protected:
  inline pattern() noexcept {}
};

struct identifier : pattern {
  std::string name;

  inline identifier(std::string _name) : name{_name} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::identifier;
  }
};

struct literal : expression {
 protected:
  inline literal() noexcept {}
};

struct null_literal : literal {
  inline null_literal() noexcept {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::null_literal;
  }
};

struct bool_literal : literal {
  bool value;

  inline bool_literal(bool _value) noexcept : value{_value} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::bool_literal;
  }
};

template <typename number_type>
struct number_literal : literal {
  number_type number;

  inline number_literal(number_type _number) : number{_number} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::number_literal;
  }
};

struct string_literal : literal {
  std::string string;

  inline string_literal(std::string _string) : string{_string} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::string_literal;
  }
};

struct raw_literal : literal {
  std::string raw;

  inline raw_literal(std::string _raw) : raw{_raw} {}
  inline constexpr ast_node_type type() const noexcept {
    return ast_node_type::raw_literal;
  }
};

}  // namespace ast

}  // namespace jscodegen

#endif  // jscodegen_ast_hpp