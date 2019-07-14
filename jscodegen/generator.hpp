#ifndef jscodegen_generator_hpp
#define jscodegen_generator_hpp

#include <sstream>
#include <type_traits>

#include "../utils/string.hpp"
#include "ast.hpp"
#include "lazy_tree.hpp"
#include "source_loc.hpp"

namespace jscodegen {

struct generator {
  struct {
    std::string indent{"  "};
    std::string line_end{"\n"};
  } config;

  template <typename node_type>
  inline source_range write(node_type&& ast_node) {
    const source_loc start{_loc};
    write_node(ast_node);
    return {start, _loc};
  }

  inline std::string str() const { return _buffer.str(); }

 private:
  std::ostringstream _buffer;
  source_loc _loc{1, 1};
  size_t _indent_level;

  template <typename node_type>
  inline void write_node(node_type& node) {
    if constexpr (std::is_base_of_v<ast::base, node_type>) {
      write_elem(node);
    } else if constexpr (std::is_same_v<lazy_tree, node_type>) {
      node.write(*this);
    } else {
      /* node is of sequential type */
      for (auto& elem : node) {
        write_node(elem);
      }
    }
  }

  inline void write_elem(ast::super&) { write_raw("super"); }

  inline void write_elem(ast::this_expression&) { write_raw("this"); }

  template <typename argument_type>
  inline void write_elem(ast::unary_expression<argument_type>& unary) {
    const std::string op_symbol = symbol_for(unary.op);
    write_raw(op_symbol);
    if (op_symbol.size() > 1) {
      write_raw(" ");
    }

    if (precedence_for(unary.argument.type()) <
        precedence_for(ast_node_type::unary_expression)) {
      write_raw("(");
      write_node(unary.argument);
      write_raw(")");
    } else {
      write_node(unary.argument);
    }
  }

  template <typename left_type, typename right_type>
  inline void write_elem(
      ast::assignment_expression<left_type, right_type>& assignment) {
    write_node(assignment.left);
    write_raw(" ");
    write_raw(symbol_for(assignment.op));
    write_raw(" ");
    write_node(assignment.right);
  }

  template <typename argument_type>
  inline void write_elem(ast::update_expression<argument_type>& update) {
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

  inline void write_elem(ast::identifier& identifier) {
    write_raw(identifier.name);
  }

  inline void write_elem(ast::null_literal&) { write_raw("null"); }
  inline void write_elem(ast::bool_literal& literal) {
    write_raw(literal.value ? "true" : "false");
  }
  template <typename number_type>
  inline void write_elem(ast::number_literal<number_type>& literal) {
    write_raw(std::to_string(literal.number));
  }
  inline void write_elem(ast::string_literal& literal) {
    write_raw(quoted(literal.string));
  }
  inline void write_elem(ast::raw_literal& literal) { write_raw(literal.raw); }

  void write_raw(std::string str);
};

}  // namespace jscodegen

#endif  // jscodegen_generator_hpp