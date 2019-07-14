#ifndef jscodegen_lazy_tree_hpp
#define jscodegen_lazy_tree_hpp

#include <optional>
#include <type_traits>

#include "ast.hpp"
#include "source_loc.hpp"

namespace jscodegen {

struct generator;

struct lazy_tree {
  struct empty_callback_tag {};

  struct impl_base {
    virtual ~impl_base() {}
    virtual const ast::base& get_root() const = 0;
    virtual void callback(source_range range) const = 0;
  };

  template <typename callback_type>
  struct optional_callback : impl_base {
   protected:
    inline optional_callback(callback_type callback) : _callback{callback} {}
    void callback(source_range range) const override { _callback(range); }

   private:
    callback_type _callback;
  };

  template <
      typename ast_builder_type, typename callback_type,
      typename = std::enable_if_t<
          std::is_invocable_v<ast_builder_type> &&
          std::is_base_of_v<ast::base, std::result_of_t<ast_builder_type>>>>
  struct impl : optional_callback<callback_type> {
    using root_type = std::result_of_t<ast_builder_type>;

    inline impl(ast_builder_type builder, callback_type callback)
        : optional_callback<callback_type>{callback}, _builder{builder} {}

    const ast::base& get_root() const override {
      if (!_root.has_value()) {
        _root = _builder();
      }
      return _root.value();
    }

   private:
    ast_builder_type _builder;
    mutable std::optional<root_type> _root;
  };

  template <typename ast_builder_type>
  inline lazy_tree(ast_builder_type builder)
      : _impl{std::make_unique<impl<ast_builder_type, empty_callback_tag>>(
            builder, empty_callback_tag{})} {}

  template <typename ast_builder_type, typename callback_type>
  inline lazy_tree(ast_builder_type builder, callback_type callback)
      : _impl{std::make_unique<impl<ast_builder_type, callback_type>>(
            builder, callback)} {}

  inline void write(generator& g) const { _impl->write(g); }
  inline ast_node_type type() const { return _impl->type(); }

 private:
  std::unique_ptr<impl_base> _impl;
};

template <>
struct lazy_tree::optional_callback<lazy_tree::empty_callback_tag> : impl_base {
 protected:
  inline optional_callback(empty_callback_tag) noexcept {}
  void callback(source_range range) const override {}
};

}  // namespace jscodegen

#endif  // jscodegen_lazy_tree_hpp
