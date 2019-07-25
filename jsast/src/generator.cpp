#include "generator.hpp"

namespace jsast {

void generator::write_raw(const std::string& str) {
  _buffer << str;

  // UTF-8 line counting
  for (size_t i{0}; i < str.length(); i++) {
    const auto c{static_cast<uint8_t>(str[i])};
    if ((c & 0xE0) == 0xC0) {
      i += 1;
    } else if ((c & 0xF0) == 0xE0) {
      i += 2;
    } else if ((c & 0xF8) == 0xF0) {
      i += 3;
    } else if ((c & 0xFC) == 0xF8) {
      i += 4;
    } else if ((c & 0xFE) == 0xFC) {
      i += 5;
    }

    if (c == '\n') {
      _loc.line++;
      _loc.column = 1;
    } else {
      _loc.column++;
    }
  }
}

template <typename parent_type, typename node_type>
inline bool binary_operand_needs_parenthesis_by_operator(
    const parent_type& parent, const node_type& node,
    binary_operand_location loc) {
  switch (loc) {
    case binary_operand_location::left:
      return precedence_for(node.op) < precedence_for(parent.op);
    case binary_operand_location::right:
      return precedence_for(node.op) <= precedence_for(parent.op);
  }
}

template <typename parent_type>
inline bool binary_operand_needs_parenthesis(const parent_type& parent,
                                             const ast::node& node,
                                             binary_operand_location loc) {
  const auto node_precedence{precedence_for(node)};
  if (node_precedence == precedence_needs_parentheses) {
    return true;
  } else if (node_precedence != precedence<parent_type>) {
    return node_precedence < precedence<parent_type>;
  } else if (node.is<ast::binary_expression>()) {
    const auto& binary{node.as<ast::binary_expression>()};
    if constexpr (std::is_same_v<parent_type, ast::binary_expression>) {
      if (binary.op == binary_op::power && parent.op == binary_op::power) {
        return loc != binary_operand_location::right;
      }
    }
    return binary_operand_needs_parenthesis_by_operator(parent, binary, loc);
  } else if (node.is<ast::logical_expression>()) {
    const auto& logical{node.as<ast::logical_expression>()};
    return binary_operand_needs_parenthesis_by_operator(parent, logical, loc);
  } else {
    return false;
  }
}

template <typename parent_type>
void generator::write_binary_operand(const parent_type& parent,
                                     const ast::node& node,
                                     binary_operand_location loc) {
  if (binary_operand_needs_parenthesis(parent, node, loc)) {
    write_elems("(", node, ")");
  } else {
    write_elems(node);
  }
}

template void generator::write_binary_operand<ast::binary_expression>(
    const ast::binary_expression&, const ast::node&, binary_operand_location);
template void generator::write_binary_operand<ast::logical_expression>(
    const ast::logical_expression&, const ast::node&, binary_operand_location);

}  // namespace jsast