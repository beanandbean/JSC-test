#ifndef jsast_ast_node_hpp
#define jsast_ast_node_hpp

#include <memory>
#include <typeindex>

namespace jsast {

struct generator;

namespace ast {

struct base;

struct node {
  friend generator;

  struct impl_base {
    friend node;

    virtual ~impl_base() noexcept {}
    virtual const base& get() const = 0;
    virtual std::type_index type() const = 0;

   private:
    virtual void write_to(generator& g) const = 0;
  };

  template <typename node_type,
            typename = std::enable_if<std::is_base_of_v<base, node_type>>>
  struct impl : impl_base {
    inline impl(node_type&& node) : _node{std::forward<node_type>(node)} {}

    const base& get() const override { return _node; }
    std::type_index type() const override { return typeid(node_type); }
    void write_to(generator& g) const override;

   protected:
    node_type _node;
  };

  template <typename node_type, typename callback_type,
            typename = std::enable_if<std::is_base_of_v<base, node_type>>>
  struct impl_with_callback : impl<node_type> {
    inline impl_with_callback(node_type&& node, callback_type callback)
        : impl<node_type>{std::forward<node_type>(node)}, _callback{callback} {}

    void write_to(generator& g) const override;

   private:
    callback_type _callback;
  };

  template <typename node_type>
  inline node(node_type&& node)
      : _impl{
            std::make_unique<impl<node_type>>(std::forward<node_type>(node))} {}

  template <typename node_type, typename callback_type>
  inline node(node_type&& node, callback_type callback)
      : _impl{std::make_unique<impl_with_callback<node_type, callback_type>>(
            std::forward<node_type>(node), callback)} {}

  inline const base& get() const { return _impl->get(); }
  inline std::type_index type() const { return _impl->type(); }

 private:
  std::unique_ptr<impl_base> _impl;

  inline void write_to(generator& g) const { _impl->write_to(g); }
};

}  // namespace ast

template <typename node_type>
inline bool node_is(const ast::node& node) {
  return node.type() == std::type_index{typeid(node_type)};
}

}  // namespace jsast

#endif  // jsast_ast_node_hpp