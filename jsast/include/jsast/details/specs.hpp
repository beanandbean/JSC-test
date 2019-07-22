#ifndef jsast_specs_hpp
#define jsast_specs_hpp

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

static constexpr const char* _assignment_op_symbol_map[]{
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
inline constexpr const char* symbol_for(assignment_op op) noexcept {
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

static constexpr const char* _binary_op_symbol_map[]{
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
inline constexpr const char* symbol_for(binary_op op) noexcept {
  return _binary_op_symbol_map[static_cast<uint8_t>(op)];
}

static constexpr size_t _binary_op_precedence_map[]{
    11 /* add */,
    11 /* subtract */,
    12 /* multiply */,
    12 /* divide */,
    12 /* modulus */,
    13 /* power */,
    10 /* lshift */,
    10 /* rshift */,
    10 /* unsigned_rshift */,
    7 /* bitwise_and */,
    5 /* bitwise_or */,
    6 /* bitwise_xor */,
    8 /* equal */,
    8 /* not_equal */,
    8 /* strict_equal */,
    8 /* strict_not_equal */,
    9 /* less */,
    9 /* less_equal */,
    9 /* greater */,
    9 /* greater_equal */,
    9 /* in */,
    9 /* instance_of */
};
inline constexpr size_t precedence_for(binary_op op) noexcept {
  return _binary_op_precedence_map[static_cast<uint8_t>(op)];
}

enum class logical_op : uint8_t { logical_and, logical_or };

static constexpr const char* _logical_op_symbol_map[]{
    "&&" /* logical_and */, "||" /* logical_or */
};
inline constexpr const char* symbol_for(logical_op op) noexcept {
  return _logical_op_symbol_map[static_cast<uint8_t>(op)];
}

static constexpr size_t _logical_op_precedence_map[]{
    4 /* logical_and */, 3 /* logical_or */
};
inline constexpr size_t precedence_for(logical_op op) noexcept {
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

static constexpr const char* _unary_op_symbol_map[]{
    "+" /* positive */,
    "-" /* negative */,
    "!" /* logical_not */,
    "~" /* bitwise_not */,
    "typeof" /* type_of */,
    "void" /* void_op */,
    "delete" /* delete_op */
};
inline constexpr const char* symbol_for(unary_op op) noexcept {
  return _unary_op_symbol_map[static_cast<uint8_t>(op)];
}

enum class update_op : uint8_t { increment, decrement };

static constexpr const char* _update_op_symbol_map[]{
    "++" /* increment */, "--" /* decrement */
};
inline constexpr const char* symbol_for(update_op op) noexcept {
  return _update_op_symbol_map[static_cast<uint8_t>(op)];
}

enum class variable_declaration_type : uint8_t { var, let, constant };

static constexpr const char* _variable_declaration_type_symbol_map[]{
    "var" /* var */, "let" /* let */, "const" /* constant */
};
inline constexpr const char* symbol_for(
    variable_declaration_type type) noexcept {
  return _variable_declaration_type_symbol_map[static_cast<uint8_t>(type)];
}

enum class unary_op_location { prefix, suffix };
enum class binary_operand_location { left, right };

}  // namespace jsast

#endif  // jsast_specs_hpp