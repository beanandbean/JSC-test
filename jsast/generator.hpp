#ifndef jsast_generator_hpp
#define jsast_generator_hpp

#include <sstream>
#include <type_traits>

#include "../utils/string.hpp"
#include "ast.hpp"
#include "ast_specs.hpp"
#include "source_loc.hpp"

namespace jsast {

struct generator {
  template <typename, typename>
  friend struct ast::node::impl;

  struct {
    std::string indent{"  "};
    std::string line_end{"\n"};
  } config;

  template <typename node_type>
  inline source_range write(node_type&& ast_node) {
    const auto start{_loc};
    write_node(std::forward<node_type>(ast_node));
    return {start, _loc};
  }

  inline std::string str() const { return _buffer.str(); }

 private:
  std::ostringstream _buffer;
  source_loc _loc{1, 1};
  size_t _indent_level;

  inline void write_node(const ast::node& node) { node.write_to(*this); }

  inline void write_node(const ast::super&) { write_raw("super"); }

  inline void write_node(const ast::this_expression&) { write_raw("this"); }

  inline void write_node(const ast::unary_expression& unary) {
    const auto op_symbol{symbol_for(unary.op)};
    write_raw(op_symbol);
    if (op_symbol.size() > 1) {
      write_raw(" ");
    }

    if (precedence_for(unary.argument) < precedence<ast::unary_expression>) {
      write_raw("(");
      write_node(unary.argument);
      write_raw(")");
    } else {
      write_node(unary.argument);
    }
  }

  inline void write_node(const ast::binary_expression& binary) {
    if (binary.op == binary_op::in) {
      // Avoids confusion in for-loop initializers
      write_raw("(");
      write_binary(binary);
      write_raw(")");
    } else {
      write_binary(binary);
    }
  }

  inline void write_node(const ast::assignment_expression& assignment) {
    write_node(assignment.left);
    write_raw(" ");
    write_raw(symbol_for(assignment.op));
    write_raw(" ");
    write_node(assignment.right);
  }

  inline void write_node(const ast::update_expression& update) {
    switch (update.loc) {
      case unary_op_location::prefix:
        write_raw(symbol_for(update.op));
        write_node(update.argument);
        break;
      case unary_op_location::suffix:
        write_node(update.argument);
        write_raw(symbol_for(update.op));
        break;
    }
  }

  inline void write_node(const ast::logical_expression& logical) {
    write_binary(logical);
  }

  inline void write_node(const ast::identifier& identifier) {
    write_raw(identifier.name);
  }

  inline void write_node(const ast::null_literal&) { write_raw("null"); }
  inline void write_node(const ast::bool_literal& literal) {
    write_raw(literal.value ? "true" : "false");
  }

  inline void write_node(const ast::number_literal& literal) {
    write_raw(literal.number);
  }
  inline void write_node(const ast::string_literal& literal) {
    write_raw(quoted(literal.string));
  }
  inline void write_node(const ast::raw_literal& literal) {
    write_raw(literal.raw);
  }

  template <typename node_type>
  inline void write_binary(const node_type& node) {
    write_binary_operand(node, node.left, binary_operand_location::left);
    write_raw(" ");
    write_raw(symbol_for(node.op));
    write_raw(" ");
    write_binary_operand(node, node.right, binary_operand_location::right);
  }
  template <typename parent_type>
  void write_binary_operand(const parent_type& parent, const ast::node& node,
                            binary_operand_location loc);

  void write_raw(std::string str);
};

}  // namespace jsast

#endif  // jsast_generator_hpp