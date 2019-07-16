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
  template <typename, typename, typename>
  friend struct ast::node::impl_with_callback;

  struct {
    std::string indent{"  "};
    std::string line_end{"\n"};
  } config;

  template <typename node_type>
  inline void write(const node_type& node) {
    write_statement(node);
  }

  inline std::string str() const { return _buffer.str(); }

 private:
  std::ostringstream _buffer;
  source_loc _loc{1, 1};
  size_t _indent_level{0};

  template <typename callable_type>
  inline source_range with_range(callable_type callable) {
    const auto start{_loc};
    callable();
    return {start, _loc};
  }

  inline void write_node(const ast::node& node) { node.write_to(*this); }

  inline void write_node(const ast::super&) { write_raw("super"); }

  inline void write_node(const ast::empty_statement&) { write_raw(";"); }

  inline void write_node(const ast::expression_statement& statement) {
    write_node(statement.expression);
    write_raw(";");
  }

  inline void write_node(const ast::labeled_statement& statement) {
    write_node(statement.label);
    write_raw(": ");
    write_node(statement.body);
  }

  inline void write_node(const ast::break_statement& statement) {
    write_control_interrupt(statement, "break");
  }

  inline void write_node(const ast::continue_statement& statement) {
    write_control_interrupt(statement, "continue");
  }

  inline void write_node(const ast::with_statement& statement) {
    write_raw("with (");
    write_node(statement.object);
    write_raw(") ");
    write_node(statement.body);
  }

  inline void write_node(const ast::return_statement& statement) {
    write_raw("return");
    if (statement.argument.has_value()) {
      write_raw(" ");
      write_node(statement.argument.value());
    }
    write_raw(";");
  }

  inline void write_node(const ast::throw_statement& statement) {
    write_raw("throw ");
    write_node(statement.argument);
    write_raw(";");
  }

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

  inline void write_node(const ast::conditional_expression& conditional) {
    if (precedence_for(conditional.test) >
        precedence<ast::conditional_expression>) {
      write_node(conditional.test);
    } else {
      write_raw("(");
      write_node(conditional.test);
      write_raw(")");
    }
    write_raw(" ? ");
    write_node(conditional.consequent);
    write_raw(" : ");
    write_node(conditional.alternate);
  }

  inline void write_node(const ast::call_expression& call) {
    if (precedence_for(call.callee) < precedence<ast::call_expression>) {
      write_raw("(");
      write_node(call.callee);
      write_raw(")");
    } else {
      write_node(call.callee);
    }
    write_sequence(call.arguments);
  }

  inline void write_node(const ast::new_expression& call) {
    auto needs_parenthesis{precedence_for(call.callee) <
                           precedence<ast::new_expression>};
    if (!needs_parenthesis) {
      // Check for call expression
      const auto* current = &call.callee;
      while (node_is<ast::member_expression>(*current)) {
        current =
            &static_cast<const ast::member_expression&>(current->get()).object;
      }
      if (node_is<ast::call_expression>(*current)) {
        needs_parenthesis = true;
      }
    }

    write_raw("new ");
    if (needs_parenthesis) {
      write_raw("(");
      write_node(call.callee);
      write_raw(")");
    } else {
      write_node(call.callee);
    }
    write_sequence(call.arguments);
  }

  inline void write_node(const ast::member_expression& member) {
    if (precedence_for(member.object) < precedence<ast::member_expression>) {
      write_raw("(");
      write_node(member.object);
      write_raw(")");
    } else {
      write_node(member.object);
    }

    // Test for identifier
    auto is_identifier{member.property.size() > 0};
    if (is_identifier) {
      for (size_t i{0}; i < member.property.size(); i++) {
        const auto ch{static_cast<uint8_t>(member.property[i])};
        if ((ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z') && ch != '_' &&
            ch != '$' && (i == 0 || ch < '0' || ch > '9')) {
          is_identifier = false;
          break;
        }
      }
    }

    if (is_identifier) {
      write_raw(".");
      write_raw(member.property);
    } else {
      write_raw("[");
      write_raw(quoted(member.property));
      write_raw("]");
    }
  }

  inline void write_node(const ast::computed_member_expression& member) {
    if (precedence_for(member.object) < precedence<ast::member_expression>) {
      write_raw("(");
      write_node(member.object);
      write_raw(")");
    } else {
      write_node(member.object);
    }
    write_raw("[");
    write_node(member.property);
    write_raw("]");
  }

  inline void write_node(const ast::yield_expression& yield) {
    write_raw("yield");
    if (yield.argument.has_value()) {
      write_raw(" ");
      write_node(yield.argument.value());
    }
  }

  inline void write_node(const ast::delegate_yield_expression& yield) {
    write_raw("yield* ");
    write_node(yield.argument);
  }

  inline void write_node(const ast::await_expression& await) {
    write_raw("await ");
    write_node(await.argument);
  }

  inline void write_node(const ast::identifier& identifier) {
    write_raw(identifier.name);
  }

  inline void write_node(const ast::assignment_pattern& assignment) {
    write_node(assignment.left);
    write_raw(" = ");
    write_node(assignment.right);
  }

  inline void write_node(const ast::rest_element& rest) {
    write_raw("...");
    write_node(rest.argument);
  }

  inline void write_node(const ast::spread_element& spread) {
    write_raw("...");
    write_node(spread.argument);
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
  inline void write_statement(const node_type& node) {
    write_indent();
    write_node(node);
    write_line_end();
  }

  template <typename node_type>
  inline void write_control_interrupt(const node_type& node, std::string name) {
    write_raw(name);
    if (node.label.has_value()) {
      write_raw(" ");
      write_node(node.label.value());
    }
    write_raw(";");
  }

  inline void write_sequence(const node_vector& nodes) {
    write_raw("(");
    if (nodes.size() > 0) {
      write_node(nodes[0]);
      for (size_t i{1}; i < nodes.size(); i++) {
        write_raw(", ");
        write_node(nodes[i]);
      }
    }
    write_raw(")");
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

  inline void write_line_end() { write_raw(config.line_end); }
  inline void write_indent() {
    for (size_t i{0}; i < _indent_level; i++) {
      write_raw(config.indent);
    }
  }

  void write_raw(std::string str);
};

}  // namespace jsast

#endif  // jsast_generator_hpp