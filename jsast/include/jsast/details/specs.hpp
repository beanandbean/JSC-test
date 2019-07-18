#ifndef jsast_specs_hpp
#define jsast_specs_hpp

#include <string>
#include <unordered_map>

namespace jsast {

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
  static std::unordered_map<assignment_op, std::string> symbol_map{
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
  static std::unordered_map<binary_op, std::string> symbol_map{
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
  static std::unordered_map<binary_op, size_t> precedence_map{
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
  static std::unordered_map<logical_op, std::string> symbol_map{
      {logical_op::logical_and, "&&"}, {logical_op::logical_or, "||"}};
  return symbol_map[op];
}

inline size_t precedence_for(logical_op op) noexcept {
  static std::unordered_map<logical_op, size_t> precedence_map{
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
  static std::unordered_map<unary_op, std::string> symbol_map{
      {unary_op::positive, "+"},      {unary_op::negative, "-"},
      {unary_op::logical_not, "!"},   {unary_op::bitwise_not, "~"},
      {unary_op::type_of, "typeof"},  {unary_op::void_op, "void"},
      {unary_op::delete_op, "delete"}};
  return symbol_map[op];
}

enum class update_op { increment, decrement };

inline std::string symbol_for(update_op op) noexcept {
  static std::unordered_map<update_op, std::string> symbol_map{
      {update_op::increment, "++"}, {update_op::decrement, "--"}};
  return symbol_map[op];
}

enum class variable_declaration_type { var, let, constant };

inline std::string symbol_for(variable_declaration_type type) noexcept {
  static std::unordered_map<variable_declaration_type, std::string> symbol_map{
      {variable_declaration_type::var, "var"},
      {variable_declaration_type::let, "let"},
      {variable_declaration_type::constant, "const"}};
  return symbol_map[type];
}

enum class unary_op_location { prefix, suffix };
enum class binary_operand_location { left, right };

}  // namespace jsast

#endif  // jsast_specs_hpp