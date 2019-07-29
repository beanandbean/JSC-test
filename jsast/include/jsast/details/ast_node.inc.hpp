#ifndef jsast_ast_node_inc
#define jsast_ast_node_inc

namespace jsast::ast {

template <typename node_type, typename enabled>
void node::impl<node_type, enabled>::write_to(generator& g) const {
  g.write_elems(_node);
}

template <typename node_type, typename callback_type, typename enabled>
void node::impl_with_callback<node_type, callback_type, enabled>::write_to(
    generator& g) const {
  _callback(g.with_range(
      [this, &g]() { node::impl<node_type, enabled>::write_to(g); }));
}

}  // namespace jsast::ast

#endif  // jsast_ast_node_inc