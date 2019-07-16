#ifndef jsast_node_vector
#define jsast_node_vector

#include <memory>
#include <vector>

#include "ast_node.hpp"

namespace jsast {

struct node_vector : std::vector<ast::node> {
  template <typename... arg_type>
  node_vector(arg_type&&... args) {
    push_all(std::forward<arg_type>(args)...);
  }

 private:
  inline void push_all() noexcept {}
  template <typename... arg_type>
  inline void push_all(ast::node&& n, arg_type&&... args) {
    push_back(std::move(n));
    push_all(std::forward<arg_type>(args)...);
  }
};

}  // namespace jsast

#endif  // jsast_node_vector