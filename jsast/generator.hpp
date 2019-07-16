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
  template <typename, typename, typename>
  friend struct ast::node::impl_with_callback;

  struct {
    std::string indent{"  "};
    std::string line_end{"\n"};
  } config;

  inline void write(const ast::node& node) { node.write_to(*this); }

  inline void write(const ast::super&) { write_raw("super"); }

  inline void write(const ast::this_expression&) { write_raw("this"); }

  inline void write(const ast::unary_expression& unary) {
    const auto op_symbol{symbol_for(unary.op)};
    write_raw(op_symbol);
    if (op_symbol.size() > 1) {
      write_raw(" ");
    }

    if (precedence_for(unary.argument) < precedence<ast::unary_expression>) {
      write_raw("(");
      write(unary.argument);
      write_raw(")");
    } else {
      write(unary.argument);
    }
  }

  inline void write(const ast::binary_expression& binary) {
    if (binary.op == binary_op::in) {
      // Avoids confusion in for-loop initializers
      write_raw("(");
      write_binary(binary);
      write_raw(")");
    } else {
      write_binary(binary);
    }
  }

  inline void write(const ast::assignment_expression& assignment) {
    write(assignment.left);
    write_raw(" ");
    write_raw(symbol_for(assignment.op));
    write_raw(" ");
    write(assignment.right);
  }

  inline void write(const ast::update_expression& update) {
    switch (update.loc) {
      case unary_op_location::prefix:
        write_raw(symbol_for(update.op));
        write(update.argument);
        break;
      case unary_op_location::suffix:
        write(update.argument);
        write_raw(symbol_for(update.op));
        break;
    }
  }

  inline void write(const ast::logical_expression& logical) {
    write_binary(logical);
  }

  inline void write(const ast::identifier& identifier) {
    write_raw(identifier.name);
  }

  inline void write(const ast::null_literal&) { write_raw("null"); }
  inline void write(const ast::bool_literal& literal) {
    write_raw(literal.value ? "true" : "false");
  }

  inline void write(const ast::number_literal& literal) {
    write_raw(literal.number);
  }
  inline void write(const ast::string_literal& literal) {
    write_raw(quoted(literal.string));
  }
  inline void write(const ast::raw_literal& literal) { write_raw(literal.raw); }

  inline std::string str() const { return _buffer.str(); }

 private:
  std::ostringstream _buffer;
  source_loc _loc{1, 1};
  size_t _indent_level;

  template <typename callable_type>
  inline source_range with_range(callable_type callable) {
    const auto start{_loc};
    callable();
    return {start, _loc};
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