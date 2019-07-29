#ifndef jsast_specs_hpp
#define jsast_specs_hpp

#include <array>

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

static constexpr std::array _assignment_op_symbol_map{
    "=" /* standard */,
    "+=" /* add */,
    "-=" /* subtract */,
    "*=" /* multiply */,
    "/=" /* divide */,
    "%=" /* modulus */,
    "**=" /* power */,
    "<<=" /* lshift */,
    ">>=" /* rshift */,
    ">>>=" /* unsigned_rshift */,
    "&=" /* bitwise_and */,
    "|=" /* bitwise_or */,
    "^=" /* bitwise_xor */
};
[[nodiscard]] inline constexpr const char* symbol_for(
    assignment_op op) noexcept {
  return _assignment_op_symbol_map[static_cast<uint8_t>(op)];
}

enum class binary_op : uint8_t {
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

static constexpr std::array _binary_op_symbol_map{
    "+" /* add */,
    "-" /* subtract */,
    "*" /* multiply */,
    "/" /* divide */,
    "%" /* modulus */,
    "**" /* power */,
    "<<" /* lshift */,
    ">>" /* rshift */,
    ">>>" /* unsigned_rshift */,
    "&" /* bitwise_and */,
    "|" /* bitwise_or */,
    "^" /* bitwise_xor */,
    "==" /* equal */,
    "!=" /* not_equal */,
    "===" /* strict_equal */,
    "!==" /* strict_not_equal */,
    "<" /* less */,
    "<=" /* less_equal */,
    ">" /* greater */,
    ">=" /* greater_equal */,
    "in" /* in */,
    "instanceof" /* instance_of */
};
[[nodiscard]] inline constexpr const char* symbol_for(binary_op op) noexcept {
  return _binary_op_symbol_map[static_cast<uint8_t>(op)];
}

static constexpr std::array _binary_op_precedence_map{
    11UL /* add */,
    11UL /* subtract */,
    12UL /* multiply */,
    12UL /* divide */,
    12UL /* modulus */,
    13UL /* power */,
    10UL /* lshift */,
    10UL /* rshift */,
    10UL /* unsigned_rshift */,
    7UL /* bitwise_and */,
    5UL /* bitwise_or */,
    6UL /* bitwise_xor */,
    8UL /* equal */,
    8UL /* not_equal */,
    8UL /* strict_equal */,
    8UL /* strict_not_equal */,
    9UL /* less */,
    9UL /* less_equal */,
    9UL /* greater */,
    9UL /* greater_equal */,
    9UL /* in */,
    9UL /* instance_of */
};
[[nodiscard]] inline constexpr size_t precedence_for(binary_op op) noexcept {
  return _binary_op_precedence_map[static_cast<uint8_t>(op)];
}

enum class logical_op : uint8_t { logical_and, logical_or };

static constexpr std::array _logical_op_symbol_map{
    "&&" /* logical_and */, "||" /* logical_or */
};
[[nodiscard]] inline constexpr const char* symbol_for(logical_op op) noexcept {
  return _logical_op_symbol_map[static_cast<uint8_t>(op)];
}

static constexpr std::array _logical_op_precedence_map{
    4UL /* logical_and */, 3UL /* logical_or */
};
[[nodiscard]] inline constexpr size_t precedence_for(logical_op op) noexcept {
  return _logical_op_precedence_map[static_cast<uint8_t>(op)];
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

static constexpr std::array _unary_op_symbol_map{
    "+" /* positive */,
    "-" /* negative */,
    "!" /* logical_not */,
    "~" /* bitwise_not */,
    "typeof" /* type_of */,
    "void" /* void_op */,
    "delete" /* delete_op */
};
[[nodiscard]] inline constexpr const char* symbol_for(unary_op op) noexcept {
  return _unary_op_symbol_map[static_cast<uint8_t>(op)];
}

enum class update_op : uint8_t { increment, decrement };

static constexpr std::array _update_op_symbol_map{
    "++" /* increment */, "--" /* decrement */
};
[[nodiscard]] inline constexpr const char* symbol_for(update_op op) noexcept {
  return _update_op_symbol_map[static_cast<uint8_t>(op)];
}

enum class variable_declaration_type : uint8_t { var, let, constant };

static constexpr std::array _variable_declaration_type_symbol_map{
    "var" /* var */, "let" /* let */, "const" /* constant */
};
[[nodiscard]] inline constexpr const char* symbol_for(
    variable_declaration_type type) noexcept {
  return _variable_declaration_type_symbol_map[static_cast<uint8_t>(type)];
}

enum class unary_op_location { prefix, suffix };
enum class binary_operand_location { left, right };

}  // namespace jsast

#endif  // jsast_specs_hpp