#ifndef jsast_generator_hpp
#define jsast_generator_hpp

#include <sstream>
#include <type_traits>

#include "ast.hpp"
#include "ast_specs.hpp"
#include "source_loc.hpp"
#include "utils.hpp"

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

  inline void write_elems() noexcept {}
  template <typename... arg_type>
  inline void write_elems(std::string str, arg_type&&... args) {
    write_raw(str);
    write_elems(std::forward<arg_type>(args)...);
  }
  template <typename... arg_type>
  inline void write_elems(const ast::node& node, arg_type&&... args) {
    node.write_to(*this);
    write_elems(std::forward<arg_type>(args)...);
  }
  template <
      typename node_type, typename... arg_type,
      typename = std::enable_if_t<std::is_base_of_v<ast::base, node_type>>>
  inline void write_elems(const node_type& node, arg_type&&... args) {
    write_node(node);
    write_elems(std::forward<arg_type>(args)...);
  }

  inline void write_node(const ast::super&) { write_elems("super"); }

  inline void write_node(const ast::variable_declarator& declarator) {
    write_elems(declarator.id);
    if (declarator.init.has_value()) {
      write_elems(" = ", declarator.init.value());
    }
  }

  inline void write_node(const ast::empty_statement&) { write_elems(";"); }

  inline void write_node(const ast::block_statement& statement) {
    write_block(statement.body);
  }

  inline void write_node(const ast::expression_statement& statement) {
    write_elems(statement.expression, ";");
  }

  inline void write_node(const ast::if_statement& statement) {
    write_elems("if (", statement.test, ") ", statement.consequent);
    if (statement.alternate.has_value()) {
      write_elems(" else ", statement.alternate.value());
    }
  }

  inline void write_node(const ast::labeled_statement& statement) {
    write_elems(statement.label, ": ", statement.body);
  }

  inline void write_node(const ast::break_statement& statement) {
    write_control_interrupt(statement, "break");
  }

  inline void write_node(const ast::continue_statement& statement) {
    write_control_interrupt(statement, "continue");
  }

  inline void write_node(const ast::with_statement& statement) {
    write_elems("with (", statement.object, ") ", statement.body);
  }

  inline void write_node(const ast::return_statement& statement) {
    write_elems("return");
    if (statement.argument.has_value()) {
      write_elems(" ", statement.argument.value());
    }
    write_elems(";");
  }

  inline void write_node(const ast::throw_statement& statement) {
    write_elems("throw ", statement.argument, ";");
  }

  inline void write_node(const ast::while_statement& statement) {
    write_elems("while (", statement.test, ") ", statement.body);
  }

  inline void write_node(const ast::do_while_statement& statement) {
    write_elems("do", statement.body, " while (", statement.test, ");");
  }

  inline void write_node(const ast::debugger_statement&) {
    write_elems("debugger;");
  }

  inline void write_node(const ast::variable_declaration& declaration) {
    write_variable_declaration(declaration);
    write_elems(";");
  }

  inline void write_node(const ast::this_expression&) { write_raw("this"); }

  inline void write_node(const ast::array_expression& array) {
    write_array(array.elements);
  }

  inline void write_node(const ast::sequence_expression& sequence) {
    write_sequence(sequence.expressions);
  }

  inline void write_node(const ast::unary_expression& unary) {
    const auto op_symbol{symbol_for(unary.op)};
    write_elems(op_symbol);
    if (op_symbol.size() > 1) {
      write_elems(" ");
    }

    if (precedence_for(unary.argument) < precedence<ast::unary_expression>) {
      write_elems("(", unary.argument, ")");
    } else {
      write_elems(unary.argument);
    }
  }

  inline void write_node(const ast::binary_expression& binary) {
    if (binary.op == binary_op::in) {
      // Avoids confusion in for-loop initializers
      write_elems("(");
      write_binary(binary);
      write_elems(")");
    } else {
      write_binary(binary);
    }
  }

  inline void write_node(const ast::assignment_expression& assignment) {
    write_elems(assignment.left, " ", symbol_for(assignment.op), " ",
                assignment.right);
  }

  inline void write_node(const ast::update_expression& update) {
    switch (update.loc) {
      case unary_op_location::prefix:
        write_elems(symbol_for(update.op), update.argument);
        break;
      case unary_op_location::suffix:
        write_elems(update.argument, symbol_for(update.op));
        break;
    }
  }

  inline void write_node(const ast::logical_expression& logical) {
    write_binary(logical);
  }

  inline void write_node(const ast::conditional_expression& conditional) {
    if (precedence_for(conditional.test) >
        precedence<ast::conditional_expression>) {
      write_elems(conditional.test);
    } else {
      write_elems("(", conditional.test, ")");
    }
    write_elems(" ? ", conditional.consequent, " : ", conditional.alternate);
  }

  inline void write_node(const ast::call_expression& call) {
    if (precedence_for(call.callee) < precedence<ast::call_expression>) {
      write_elems("(", call.callee, ")");
    } else {
      write_elems(call.callee);
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

    write_elems("new ");
    if (needs_parenthesis) {
      write_elems("(", call.callee, ")");
    } else {
      write_elems(call.callee);
    }
    write_sequence(call.arguments);
  }

  inline void write_node(const ast::member_expression& member) {
    if (precedence_for(member.object) < precedence<ast::member_expression>) {
      write_elems("(", member.object, ")");
    } else {
      write_elems(member.object);
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
      write_elems(".", member.property);
    } else {
      write_elems("[", utils::quoted(member.property), "]");
    }
  }

  inline void write_node(const ast::computed_member_expression& member) {
    if (precedence_for(member.object) < precedence<ast::member_expression>) {
      write_elems("(", member.object, ")");
    } else {
      write_elems(member.object);
    }
    write_elems("[", member.property, "]");
  }

  inline void write_node(const ast::yield_expression& yield) {
    write_elems("yield");
    if (yield.argument.has_value()) {
      write_elems(" ", yield.argument.value());
    }
  }

  inline void write_node(const ast::delegate_yield_expression& yield) {
    write_elems("yield* ", yield.argument);
  }

  inline void write_node(const ast::await_expression& await) {
    write_elems("await ", await.argument);
  }

  inline void write_node(const ast::meta_property& meta) {
    write_elems(meta.meta, ".", meta.property);
  }

  inline void write_node(const ast::identifier& identifier) {
    write_elems(identifier.name);
  }

  inline void write_node(const ast::array_pattern& array) {
    write_array(array.elements);
  }

  inline void write_node(const ast::assignment_pattern& assignment) {
    write_elems(assignment.left, " = ", assignment.right);
  }

  inline void write_node(const ast::rest_element& rest) {
    write_elems("...", rest.argument);
  }

  inline void write_node(const ast::spread_element& spread) {
    write_elems("...", spread.argument);
  }

  inline void write_node(const ast::null_literal&) { write_elems("null"); }

  inline void write_node(const ast::bool_literal& literal) {
    write_elems(literal.value ? "true" : "false");
  }

  inline void write_node(const ast::number_literal& literal) {
    write_elems(literal.number);
  }

  inline void write_node(const ast::string_literal& literal) {
    write_elems(utils::quoted(literal.string));
  }

  inline void write_node(const ast::raw_literal& literal) {
    write_elems(literal.raw);
  }

  template <typename node_type>
  inline void write_statement(const node_type& node) {
    write_indent();
    write_elems(node);
    write_line_end();
  }

  inline void write_block(const utils::move_vector<ast::node>& body) {
    write_elems("{");
    if (body.size() > 0) {
      write_line_end();
      _indent_level++;
      for (const auto& statement : body) {
        write_statement(statement);
      }
      _indent_level--;
      write_indent();
    }
    write_elems("}");
  }

  inline void write_variable_declaration(
      const ast::variable_declaration& declaration) {
    write_elems(symbol_for(declaration.kind), " ");
    if (declaration.declarations.size() > 0) {
      write_elems(declaration.declarations[0]);
      for (size_t i{1}; i < declaration.declarations.size(); i++) {
        write_elems(", ", declaration.declarations[i]);
      }
    }
  }

  template <typename node_type>
  inline void write_control_interrupt(const node_type& node, std::string name) {
    write_elems(name);
    if (node.label.has_value()) {
      write_elems(" ", node.label.value());
    }
    write_elems(";");
  }

  inline void write_sequence(const utils::move_vector<ast::node>& nodes) {
    write_elems("(");
    if (nodes.size() > 0) {
      write_elems(nodes[0]);
      for (size_t i{1}; i < nodes.size(); i++) {
        write_elems(", ", nodes[i]);
      }
    }
    write_elems(")");
  }

  inline void write_array(
      const utils::move_vector<std::optional<ast::node>>& nodes) {
    write_elems("[");
    const size_t length{nodes.size()};
    if (length > 0) {
      for (size_t i{0}; i < length; i++) {
        if (nodes[i].has_value()) {
          write_elems(nodes[i].value());
          if (i < length - 1) {
            write_elems(", ");
          }
        } else {
          write_elems(", ");
        }
      }
    }
    write_elems("]");
  }

  template <typename node_type>
  inline void write_binary(const node_type& node) {
    write_binary_operand(node, node.left, binary_operand_location::left);
    write_elems(" ", symbol_for(node.op), " ");
    write_binary_operand(node, node.right, binary_operand_location::right);
  }
  template <typename parent_type>
  void write_binary_operand(const parent_type& parent, const ast::node& node,
                            binary_operand_location loc);

  inline void write_line_end() { write_elems(config.line_end); }
  inline void write_indent() {
    for (size_t i{0}; i < _indent_level; i++) {
      write_elems(config.indent);
    }
  }

  void write_raw(std::string str);
};  // namespace jsast

}  // namespace jsast

#endif  // jsast_generator_hpp