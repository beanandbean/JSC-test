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
    write_raw(std::move(str));
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

  inline void write_node(const ast::program& program) {
    const auto length = program.body.size();
    if (length > 1) {
      for (size_t i{0}; i < length; i++) {
        const auto& node = program.body[i];
        if (i == 0) {
          write_elems(node);
          write_line_end();
        } else if (i == length - 1) {
          write_indent();
          write_elems(node);
        } else {
          write_statement(node);
        }
      }
    } else if (length == 1) {
      write_elems(program.body[0]);
    }
  }

  inline void write_node(const ast::super&) { write_elems("super"); }

  inline void write_node(const ast::member_identifier& identifier) {
    // This shall not be called.
    write_member<false>(identifier);
  }

  inline void write_node(const ast::property& property) {
    write_member<false>(property.key);
    write_elems(": ", property.value);
  }

  inline void write_node(const ast::switch_case& case_node) {
    if (case_node.test.has_value()) {
      write_elems("case ", case_node.test.value(), ":");
    } else {
      write_elems("default:");
    }
    if (case_node.consequent.size() > 0) {
      write_line_end();
      _indent_level++;
      for (size_t i{0}; i < case_node.consequent.size() - 1; i++) {
        write_statement(case_node.consequent[i]);
      }
      write_indent();
      write_elems(case_node.consequent.back());
      _indent_level--;
    }
  }

  inline void write_node(const ast::catch_clause& clause) {
    write_elems("catch ");
    if (clause.pattern.has_value()) {
      write_elems("(", clause.pattern.value(), ") ");
    }
    write_elems(clause.body);
  }

  inline void write_node(const ast::variable_declarator& declarator) {
    write_elems(declarator.id);
    if (declarator.init.has_value()) {
      write_elems(" = ", declarator.init.value());
    }
  }

  inline void write_node(const ast::template_element& element) {
    write_elems(utils::backquoted(element.value));
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

  inline void write_node(const ast::switch_statement& statement) {
    write_elems("switch (", statement.discriminant, ") ");
    write_block(statement.cases);
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

  inline void write_node(const ast::try_statement& statement) {
    write_elems("try ", statement.block);
    if (statement.handler.has_value()) {
      write_elems(" ", statement.handler.value());
    }
    if (statement.finalizer.has_value()) {
      write_elems(" finally ", statement.finalizer.value());
    }
  }

  inline void write_node(const ast::while_statement& statement) {
    write_elems("while (", statement.test, ") ", statement.body);
  }

  inline void write_node(const ast::do_while_statement& statement) {
    write_elems("do", statement.body, " while (", statement.test, ");");
  }

  inline void write_node(const ast::for_statement& statement) {
    write_elems("for (");
    if (statement.init.has_value()) {
      const auto& init = statement.init.value();
      if (init.is<ast::variable_declaration>()) {
        write_variable_declaration(init.as<ast::variable_declaration>());
      } else {
        write_elems(init);
      }
    }
    write_elems("; ");
    if (statement.test.has_value()) {
      write_elems(statement.test.value());
    }
    write_elems("; ");
    if (statement.update.has_value()) {
      write_elems(statement.update.value());
    }
    write_elems(") ", statement.body);
  }

  inline void write_node(const ast::for_in_statement& statement) {
    write_for_iterator_statement(statement, "in");
  }

  inline void write_node(const ast::for_of_statement& statement) {
    write_for_iterator_statement(statement, "of");
  }

  inline void write_node(const ast::debugger_statement&) {
    write_elems("debugger;");
  }

  inline void write_node(const ast::variable_declaration& declaration) {
    write_variable_declaration(declaration);
    write_elems(";");
  }

  inline void write_node(const ast::function_declaration& declaration) {
    if (declaration.async) {
      write_elems("async ");
    }
    if (declaration.generator) {
      write_elems("function* ");
    } else {
      write_elems("function ");
    }
    write_elems(declaration.id);
    write_function_body(declaration.params, declaration.body);
  }

  inline void write_node(const ast::this_expression&) { write_raw("this"); }

  inline void write_node(const ast::array_expression& array) {
    write_array(array.elements);
  }

  inline void write_node(const ast::object_expression& object) {
    write_object(object);
  }

  inline void write_node(const ast::function_expression& function) {
    if (function.async) {
      write_elems("async ");
    }
    if (function.generator) {
      write_elems("function* ");
    } else {
      write_elems("function ");
    }
    if (function.id.has_value()) {
      write_elems(function.id.value());
    }
    write_function_body(function.params, function.body);
  }

  inline void write_node(const ast::arrow_function_expression& function) {
    if (function.async) {
      write_elems("async ");
    }
    if (function.params.size() == 1 &&
        function.params[0].is<ast::identifier>()) {
      write_elems(function.params[0]);
    } else {
      write_sequence(function.params);
    }
    write_elems(" => ");
    if (function.body.is<ast::object_expression>()) {
      write_elems("(", function.body, ")");
    } else {
      write_elems(function.body);
    }
  }

  inline void write_node(const ast::sequence_expression& sequence) {
    write_sequence(sequence.expressions);
  }

  inline void write_node(const ast::unary_expression& unary) {
    std::string op_symbol{symbol_for(unary.op)};
    if (op_symbol.size() > 1) {
      write_elems(std::move(op_symbol), " ");
    } else {
      write_elems(std::move(op_symbol));
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
      while (current->is<ast::member_expression>()) {
        current = &current->as<ast::member_expression>().object;
      }
      if (current->is<ast::call_expression>()) {
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
    write_member<true>(member.property);
  }

  inline void write_node(const ast::yield_expression& yield) {
    if (yield.delegate) {
      write_elems("yield*");
    } else {
      write_elems("yield");
    }
    if (yield.argument.has_value()) {
      write_elems(" ", yield.argument.value());
    }
  }

  inline void write_node(const ast::await_expression& await) {
    write_elems("await ", await.argument);
  }

  inline void write_node(const ast::template_literal& literal) {
    write_elems("`");
    for (const auto& quasi : literal.quasis) {
      if (quasi.is<ast::template_element>()) {
        write_elems(quasi);
      } else {
        write_elems("${", quasi, "}");
      }
    }
    write_elems("`");
  }

  inline void write_node(const ast::tagged_template_expression& expr) {
    if (precedence_for(expr.tag) <= precedence_needs_parentheses) {
      write_elems("(", expr.tag, ")");
    } else {
      write_elems(expr.tag);
    }
    write_elems(expr.quasi);
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

  inline void write_node(const ast::object_pattern& object) {
    write_object(object);
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

  inline void write_node(const ast::reg_exp_literal& literal) {
    write_elems("/", literal.pattern, "/", literal.flags);
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

  template <typename node_type>
  inline void write_for_iterator_statement(const node_type& node,
                                           std::string op) {
    write_elems("for ");
    if (node.await) {
      write_elems("await ");
    }
    write_elems("(");
    if (node.left.template is<ast::variable_declaration>()) {
      write_variable_declaration(
          node.left.template as<ast::variable_declaration>());
    } else {
      write_elems(node.left);
    }
    write_elems(" ", std::move(op), " ", node.right, ") ", node.body);
  }

  inline void write_function_body(const utils::move_vector<ast::node>& params,
                                  const ast::node& body) {
    write_sequence(params);
    write_elems(" ", body);
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
    write_elems(std::move(name));
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
  inline void write_object(const node_type& node) {
    write_elems("{");
    const auto length = node.properties.size();
    if (length > 0) {
      write_line_end();
      _indent_level++;
      for (size_t i{0}; i < length; i++) {
        write_indent();
        write_elems(node.properties[i]);
        if (i < length - 1) {
          write_elems(",");
        }
        write_line_end();
      }
      _indent_level--;
      write_indent();
    }
    write_elems("}");
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

  template <bool with_dot>
  inline void write_member(const ast::member_identifier& identifier) {
    // Test for identifier
    auto is_identifier{identifier.name.size() > 0};
    if (is_identifier) {
      for (size_t i{0}; i < identifier.name.size(); i++) {
        const auto ch{static_cast<uint8_t>(identifier.name[i])};
        if ((ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z') && ch != '_' &&
            ch != '$' && (i == 0 || ch < '0' || ch > '9')) {
          is_identifier = false;
          break;
        }
      }
    }

    if (is_identifier) {
      if constexpr (with_dot) {
        write_elems(".");
      }
      write_elems(identifier.name);
    } else {
      if constexpr (with_dot) {
        write_elems("[", utils::quoted(identifier.name), "]");
      } else {
        write_elems(utils::quoted(identifier.name));
      }
    }
  }
  template <bool with_dot>
  inline void write_member(const ast::node& member) {
    if (member.is<ast::member_identifier>()) {
      write_member<with_dot>(member.as<ast::member_identifier>());
    } else {
      write_elems("[", member, "]");
    }
  }

  inline void write_line_end() { write_elems(config.line_end); }
  inline void write_indent() {
    for (size_t i{0}; i < _indent_level; i++) {
      write_elems(config.indent);
    }
  }

  void write_raw(const std::string& str);
};  // namespace jsast

}  // namespace jsast

#endif  // jsast_generator_hpp