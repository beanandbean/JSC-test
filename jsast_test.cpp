#include <iostream>

#include "jsast/jsast.hpp"

int main() {
  jsast::generator gen;

  gen.write(jsast::ast::assignment_expression{
      jsast::ast::identifier{"value"}, jsast::assignment_op::multiply,
      jsast::ast::raw_literal{"1e-30"}});
  gen.write(jsast::ast::assignment_expression{
      jsast::ast::identifier{"value"}, jsast::assignment_op::standard,
      jsast::ast::logical_expression{
          jsast::ast::binary_expression{
              jsast::ast::binary_expression{
                  jsast::ast::binary_expression{
                      jsast::ast::number_literal{1}, jsast::binary_op::add,
                      jsast::ast::identifier{"value"}},
                  jsast::binary_op::multiply, jsast::ast::identifier{"ratio"}},
              jsast::binary_op::greater_equal, jsast::ast::number_literal{5}},
          jsast::logical_op::logical_and,
          jsast::ast::binary_expression{
              jsast::ast::identifier{"data"},
              jsast::binary_op::strict_not_equal,
              jsast::ast::string_literal{"Failed at \"source\"!"}}}});
  gen.write(jsast::ast::unary_expression{jsast::unary_op::delete_op,
                                         jsast::ast::identifier{"value"}});

  std::cout << gen.str() << "\n";

  return 0;
}