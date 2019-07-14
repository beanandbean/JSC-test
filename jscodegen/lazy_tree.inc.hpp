#ifndef jscodegen_lazy_tree_inc
#define jscodegen_lazy_tree_inc

namespace jscodegen {

template <typename ast_builder_type, typename callback_type>
void lazy_tree::impl<ast_builder_type, callback_type>::build() {
  _root = _builder();
  this->_type = extract_node_type(_root.value());
}

template <typename ast_builder_type, typename callback_type>
void lazy_tree::impl<ast_builder_type, callback_type>::write_root(
    generator& g) {
  if (_root.has_value()) {
    this->callback(g.write(_root.value()));
  }
}

}  // namespace jscodegen

#endif  // jscodegen_lazy_tree_inc