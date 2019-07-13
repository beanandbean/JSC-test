#include <iostream>

#include "jscodegen/jscodegen.hpp"

#include "utils/universal_value.hpp"

int main() {
  jscodegen::generator gen;
  gen.write_raw_literal("1e-30");
  gen.write_string_literal("Hello \"wor-\nld\" \\ very good!");

  std::cout << gen.str();

  return 0;
}