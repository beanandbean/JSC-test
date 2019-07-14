#include <iostream>

#include "jscodegen/jscodegen.hpp"

int main() {
  jscodegen::generator gen;

  auto tree_gen = []() {
    return jscodegen::lazy_tree{
        []() { return jscodegen::ast::identifier{"value"}; }};
  };

  gen.write(jscodegen::ast::assignment_expression{
      tree_gen(), jscodegen::assignment_op::multiply,
      jscodegen::ast::raw_literal{"1e-30"}});
  gen.write(jscodegen::ast::assignment_expression{
      tree_gen(), jscodegen::assignment_op::add,
      jscodegen::ast::update_expression{jscodegen::update_op::increment,
                                        tree_gen(),
                                        jscodegen::unary_op_location::prefix}});
  gen.write(jscodegen::ast::unary_expression{jscodegen::unary_op::delete_op,
                                             tree_gen()});

  std::cout << gen.str() << "\n";

  return 0;
}