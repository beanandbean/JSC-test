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

    inline void write(generator& g) {
      if (!_type.has_value()) {
        build();
      }
      write_root(g);
    }

    inline ast_node_type type() {
      if (!_type.has_value()) {
        build();
      }
      return _type.value_or(ast_node_type::unexpected_node);
    }

   protected:
    std::optional<ast_node_type> _type;

    virtual void build() = 0;
    virtual void write_root(generator& g) = 0;
  };

  template <typename callback_type>
  struct optional_callback : impl_base {
   protected:
    inline optional_callback(callback_type callback) : _callback{callback} {}
    inline void callback(source_range range) const { _callback(range); }

   private:
    callback_type _callback;
  };

  template <typename ast_builder_type, typename callback_type>
  struct impl : optional_callback<callback_type> {
    inline impl(ast_builder_type builder, callback_type callback)
        : optional_callback<callback_type>{callback}, _builder{builder} {}

    void build() override;
    void write_root(generator& g) override;

   private:
    ast_builder_type _builder;
    std::optional<decltype(_builder())> _root;

    template <typename root_type>
    static inline constexpr ast_node_type extract_node_type(root_type& root) {
      if constexpr (std::is_base_of_v<ast::base, root_type> ||
                    std::is_same_v<lazy_tree, root_type>) {
        return root.type();
      } else {
        // root is of sequential type
        return ast_node_type::unexpected_node;
      }
    }
  };

  template <typename ast_builder_type>
  inline lazy_tree(ast_builder_type builder)
      : _impl{std::make_unique<impl<ast_builder_type, empty_callback_tag>>(
            builder, empty_callback_tag{})} {}

  template <typename ast_builder_type, typename callback_type>
  inline lazy_tree(ast_builder_type builder, callback_type callback)
      : _impl{std::make_unique<impl<ast_builder_type, callback_type>>(
            builder, callback)} {}

  inline void write(generator& g) { _impl->write(g); }
  inline ast_node_type type() { return _impl->type(); }

 private:
  std::unique_ptr<impl_base> _impl;
};

template <>
struct lazy_tree::optional_callback<lazy_tree::empty_callback_tag> : impl_base {
 protected:
  inline optional_callback(empty_callback_tag) noexcept {}
  inline void callback(source_range range) const noexcept {}
};

}  // namespace jscodegen

#endif  // jscodegen_lazy_tree_hpp
