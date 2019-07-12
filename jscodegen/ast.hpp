#ifndef jscodegen_ast_hpp
#define jscodegen_ast_hpp

#include "source_loc.hpp"

namespace jsast::ast {

// TODO: For reference... may not actually be needed later
enum class node_type {
  program,
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
  literal,
  reg_exp_literal
};

}  // namespace jsast::ast

#endif  // jscodegen_ast_hpp