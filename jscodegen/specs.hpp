#ifndef jscodegen_specs_hpp
#define jscodegen_specs_hpp

#include <string>
#include <unordered_map>

namespace jscodegen {

enum class assignment_op : uint8_t {
  standard,
  add,
  subtract,
  multiply,
  divide,
  modulus,
  power,
  lshift,
  rshift,
  unsigned_rshift,
  bitwise_and,
  bitwise_or,
  bitwise_xor
};

inline std::string symbol_for(assignment_op op) noexcept {
  static std::unordered_map<assignment_op, std::string> symbol_map = {
      {assignment_op::standard, "="},
      {assignment_op::add, "+="},
      {assignment_op::subtract, "-="},
      {assignment_op::multiply, "*="},
      {assignment_op::divide, "/="},
      {assignment_op::modulus, "%="},
      {assignment_op::power, "**="},
      {assignment_op::lshift, "<<="},
      {assignment_op::rshift, ">>="},
      {assignment_op::unsigned_rshift, ">>>="},
      {assignment_op::bitwise_and, "&="},
      {assignment_op::bitwise_or, "|="},
      {assignment_op::bitwise_xor, "^="}};
  return symbol_map[op];
}

enum class binary_op {
  add,
  subtract,
  multiply,
  divide,
  modulus,
  power,
  lshift,
  rshift,
  unsigned_rshift,
  bitwise_and,
  bitwise_or,
  bitwise_xor,
  equal,
  not_equal,
  strict_equal,
  strict_not_equal,
  less,
  less_equal,
  greater,
  greater_equal,
  in,
  instance_of,
};

inline std::string symbol_for(binary_op op) noexcept {
  static std::unordered_map<binary_op, std::string> symbol_map = {
      {binary_op::add, "+"},
      {binary_op::subtract, "-"},
      {binary_op::multiply, "*"},
      {binary_op::divide, "/"},
      {binary_op::modulus, "%"},
      {binary_op::power, "**"},
      {binary_op::lshift, "<<"},
      {binary_op::rshift, ">>"},
      {binary_op::unsigned_rshift, ">>>"},
      {binary_op::bitwise_and, "&"},
      {binary_op::bitwise_or, "|"},
      {binary_op::bitwise_xor, "^"},
      {binary_op::equal, "=="},
      {binary_op::not_equal, "!="},
      {binary_op::strict_equal, "==="},
      {binary_op::strict_not_equal, "!=="},
      {binary_op::less, "<"},
      {binary_op::less_equal, "<="},
      {binary_op::greater, ">"},
      {binary_op::greater_equal, ">="},
      {binary_op::in, "in"},
      {binary_op::instance_of, "instanceof"}};
  return symbol_map[op];
}

inline size_t precedence_for(binary_op op) noexcept {
  static std::unordered_map<binary_op, size_t> precedence_map = {
      {binary_op::add, 11},
      {binary_op::subtract, 11},
      {binary_op::multiply, 12},
      {binary_op::divide, 12},
      {binary_op::modulus, 12},
      {binary_op::power, 13},
      {binary_op::lshift, 10},
      {binary_op::rshift, 10},
      {binary_op::unsigned_rshift, 10},
      {binary_op::bitwise_and, 7},
      {binary_op::bitwise_or, 5},
      {binary_op::bitwise_xor, 6},
      {binary_op::equal, 8},
      {binary_op::not_equal, 8},
      {binary_op::strict_equal, 8},
      {binary_op::strict_not_equal, 8},
      {binary_op::less, 9},
      {binary_op::less_equal, 9},
      {binary_op::greater, 9},
      {binary_op::greater_equal, 9},
      {binary_op::in, 9},
      {binary_op::instance_of, 9}};
  return precedence_map[op];
}

enum class logical_op { logical_and, logical_or };

inline std::string symbol_for(logical_op op) noexcept {
  static std::unordered_map<logical_op, std::string> symbol_map = {
      {logical_op::logical_and, "&&"}, {logical_op::logical_or, "||"}};
  return symbol_map[op];
}

inline size_t precedence_for(logical_op op) noexcept {
  static std::unordered_map<logical_op, size_t> precedence_map = {
      {logical_op::logical_and, 4}, {logical_op::logical_or, 3}};
  return precedence_map[op];
}

enum class unary_op {
  positive,
  negative,
  logical_not,
  bitwise_not,
  type_of,
  void_op,
  delete_op
};

inline std::string symbol_for(unary_op op) noexcept {
  static std::unordered_map<unary_op, std::string> symbol_map = {
      {unary_op::positive, "+"},      {unary_op::negative, "-"},
      {unary_op::logical_not, "!"},   {unary_op::bitwise_not, "~"},
      {unary_op::type_of, "typeof"},  {unary_op::void_op, "void"},
      {unary_op::delete_op, "delete"}};
  return symbol_map[op];
}

enum class update_op { increment, decrement };

inline std::string symbol_for(update_op op) noexcept {
  static std::unordered_map<update_op, std::string> symbol_map = {
      {update_op::increment, "++"}, {update_op::decrement, "--"}};
  return symbol_map[op];
}

enum class ast_node_type {
  unexpected_node /* should never occur! */,
  program,
  super,
  block_statement,
  class_body,
  empty_statement,
  expression_statement,
  if_statement,
  labeled_statement,
  break_statement,
  continue_statement,
  with_statement,
  switch_statement,
  return_statement,
  throw_statement,
  try_statement,
  while_statement,
  do_while_statement,
  for_statement,
  for_in_statement,
  for_of_statement,
  debugger_statement,
  function_declaration,
  function_expression,
  variable_declaration,
  variable_declarator,
  class_declaration,
  import_declaration,
  export_default_declaration,
  export_named_declaration,
  export_all_declaration,
  method_definition,
  class_expression,
  arrow_function_expression,
  this_expression,
  rest_element,
  spread_element,
  yield_expression,
  await_expression,
  template_literal,
  tagged_template_expression,
  array_expression,
  array_pattern,
  object_expression,
  property,
  object_pattern,
  sequence_expression,
  unary_expression,
  update_expression,
  assignment_expression,
  assignment_pattern,
  binary_expression,
  logical_expression,
  conditional_expression,
  new_expression,
  call_expression,
  member_expression,
  meta_property,
  identifier,
  null_literal,
  bool_literal,
  number_literal,
  string_literal,
  reg_exp_literal,
  raw_literal,
};

static constexpr size_t precedence_needs_parentheses = 17;

inline size_t precedence_for(ast_node_type node) noexcept {
  static std::unordered_map<ast_node_type, size_t> precedence_map = {
      // Definitions
      {ast_node_type::array_expression, 20},
      {ast_node_type::tagged_template_expression, 20},
      {ast_node_type::this_expression, 20},
      {ast_node_type::identifier, 20},
      {ast_node_type::null_literal, 18},
      {ast_node_type::bool_literal, 18},
      {ast_node_type::number_literal, 18},
      {ast_node_type::string_literal, 18},
      {ast_node_type::reg_exp_literal, 18},
      {ast_node_type::raw_literal, 18},
      {ast_node_type::template_literal, 20},
      {ast_node_type::super, 20},
      {ast_node_type::sequence_expression, 20},
      // Operations
      {ast_node_type::member_expression, 19},
      {ast_node_type::call_expression, 19},
      {ast_node_type::new_expression, 19},
      // Other definitions
      {ast_node_type::arrow_function_expression, precedence_needs_parentheses},
      {ast_node_type::class_expression, precedence_needs_parentheses},
      {ast_node_type::function_expression, precedence_needs_parentheses},
      {ast_node_type::object_expression, precedence_needs_parentheses},
      // Other operations
      {ast_node_type::update_expression, 16},
      {ast_node_type::unary_expression, 15},
      {ast_node_type::binary_expression, 14},
      {ast_node_type::logical_expression, 13},
      {ast_node_type::conditional_expression, 4},
      {ast_node_type::assignment_expression, 3},
      {ast_node_type::await_expression, 2},
      {ast_node_type::yield_expression, 2},
      {ast_node_type::rest_element, 1}};
  return precedence_map[node];
}

}  // namespace jscodegen

#endif  // jscodegen_specs_hpp