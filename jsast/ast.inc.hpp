#ifndef jsast_ast_inc
#define jsast_ast_inc

namespace jsast::ast {

template <typename node_type, typename enabled>
void node::impl<node_type, enabled>::write_to(generator& g) const {
  g.write_node(_node);
}

}  // namespace jsast::ast

#endif  // jsast_ast_inc