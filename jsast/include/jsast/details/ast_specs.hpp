#ifndef jsast_ast_specs_hpp
#define jsast_ast_specs_hpp

#include <typeindex>
#include <unordered_map>

#include "ast.hpp"

namespace jsast {

static constexpr size_t precedence_needs_parentheses{17};

inline size_t precedence_for_type(std::type_index node) noexcept {
  static const std::unordered_map<std::type_index, size_t> precedence_map{
      // Definitions
      {typeid(ast::array_expression), 20},
      {typeid(ast::tagged_template_expression), 20},
      {typeid(ast::sequence_expression), 20},
      {typeid(ast::this_expression), 20},
      {typeid(ast::super), 20},
      {typeid(ast::identifier), 20},
      {typeid(ast::meta_property), 20},
      {typeid(ast::template_literal), 20},
      {typeid(ast::null_literal), 18},
      {typeid(ast::bool_literal), 18},
      {typeid(ast::number_literal), 18},
      {typeid(ast::string_literal), 18},
      {typeid(ast::reg_exp_literal), 18},
      {typeid(ast::raw_literal), 18},
      // Operations
      {typeid(ast::member_expression), 19},
      {typeid(ast::call_expression), 19},
      {typeid(ast::new_expression), 19},
      // Other definitions
      {typeid(ast::object_expression), precedence_needs_parentheses},
      {typeid(ast::function_expression), precedence_needs_parentheses},
      {typeid(ast::arrow_function_expression), precedence_needs_parentheses},
      // {typeid(ast::class_expression), precedence_needs_parentheses},
      // Other operations
      {typeid(ast::update_expression), 16},
      {typeid(ast::unary_expression), 15},
      {typeid(ast::binary_expression), 14},
      {typeid(ast::logical_expression), 13},
      {typeid(ast::conditional_expression), 4},
      {typeid(ast::assignment_expression), 3},
      {typeid(ast::await_expression), 2},
      {typeid(ast::yield_expression), 2},
      {typeid(ast::rest_element), 1}};
  const auto found = precedence_map.find(node);
  if (found == precedence_map.end()) {
    /* should not happen */
    return 0;
  } else {
    return found->second;
  }
}

[[nodiscard]] inline size_t precedence_for(const ast::node& node) {
  return precedence_for_type(node.type());
}

template <typename node_type>
static const size_t precedence{precedence_for_type(typeid(node_type))};

}  // namespace jsast

#endif  // jsast_ast_specs_hpp