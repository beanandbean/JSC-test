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
    virtual base& get() = 0;
    virtual const base& get() const = 0;
    virtual std::type_index type() const = 0;

   private:
    virtual void write_to(generator& g) const = 0;
  };

  template <typename node_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  struct impl : impl_base {
    inline impl(node_type&& node) : _node{std::forward<node_type>(node)} {}

    base& get() override { return _node; }
    const base& get() const override { return _node; }
    std::type_index type() const override { return typeid(node_type); }
    void write_to(generator& g) const override;

   protected:
    node_type _node;
  };

  template <typename node_type, typename callback_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  struct impl_with_callback : impl<node_type> {
    inline impl_with_callback(node_type&& node, callback_type callback)
        : impl<node_type>{std::forward<node_type>(node)}, _callback{callback} {}

    void write_to(generator& g) const override;

   private:
    callback_type _callback;
  };

  template <typename node_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  inline node(node_type&& node)
      : _impl{
            std::make_unique<impl<node_type>>(std::forward<node_type>(node))} {}

  template <typename node_type, typename callback_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  inline node(node_type&& node, callback_type callback)
      : _impl{std::make_unique<impl_with_callback<node_type, callback_type>>(
            std::forward<node_type>(node), callback)} {}

  inline base& get() { return _impl->get(); }
  inline const base& get() const { return _impl->get(); }
  inline std::type_index type() const { return _impl->type(); }

  template <typename node_type>
  inline bool is() const {
    return type() == typeid(node_type);
  }
  template <typename node_type>
  inline node_type& as() {
    return static_cast<node_type&>(get());
  }
  template <typename node_type>
  inline const node_type& as() const {
    return static_cast<const node_type&>(get());
  }

 private:
  std::unique_ptr<impl_base> _impl;

  inline void write_to(generator& g) const { _impl->write_to(g); }
};

}  // namespace ast

}  // namespace jsast

#endif  // jsast_ast_node_hpp