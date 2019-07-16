// Reference only
enum class ast_node_type {
  unexpected_node /* should never occur! */,
  program,
  super,  // √
  class_body,
  property,
  assignment_property,
  switch_case,
  catch_clause,
  variable_declarator,
  method_definition,
  empty_statement,  // √
  block_statement,
  expression_statement,  // √
  if_statement,
  labeled_statement,   // √
  break_statement,     // √
  continue_statement,  // √
  with_statement,      // √
  switch_statement,
  return_statement,  // √
  throw_statement,
  try_statement,
  while_statement,
  do_while_statement,
  for_statement,
  for_in_statement,
  for_of_statement,
  debugger_statement,
  variable_declaration,
  function_declaration,
  class_declaration,
  import_declaration,
  export_default_declaration,
  export_named_declaration,
  export_all_declaration,
  this_expression,  // √
  array_expression,
  object_expression,
  function_expression,
  arrow_function_expression,
  sequence_expression,
  unary_expression,            // √
  binary_expression,           // √
  assignment_expression,       // √
  update_expression,           // √
  logical_expression,          // √
  conditional_expression,      // √
  call_expression,             // √
  new_expression,              // √
  member_expression,           // √
  computed_member_expression,  // √
  yield_expression,            // √
  delegate_yield_expression,   // √
  await_expression,            // √
  class_expression,
  template_literal,
  tagged_template_expression,
  meta_property,
  identifier,  // √
  object_pattern,
  array_pattern,
  assignment_pattern,  // √
  rest_element,        // √
  spread_element,      // √
  null_literal,        // √
  bool_literal,        // √
  number_literal,      // √
  string_literal,      // √
  reg_exp_literal,
  raw_literal,  // √
};