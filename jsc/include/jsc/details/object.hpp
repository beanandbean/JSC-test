#ifndef jsc_object_hpp
#define jsc_object_hpp

#include <type_traits>

#include "string.hpp"
#include "value.hpp"

namespace jsc {

struct context;
template <typename property_type>
struct property;

struct object {
  template <typename property_type>
  friend struct property;

  object(context& ctx, JSObjectRef ref);
  ~object();

  object(const object& obj);
  object& operator=(const object& obj);

  inline operator value() const { return {_ctx, _ref}; }

  property<details::string_wrapper> operator[](const std::string& name) const;
  property<unsigned int> operator[](unsigned int index) const;

  [[nodiscard]] bool is_function() const;

  template <typename object_type>
  [[nodiscard]] bool is_container() const;

  template <typename object_type>
  [[nodiscard]] object_type* get_contained() const {
    assert(is_container<object_type>());
    return static_cast<object_type*>(JSObjectGetPrivate(_ref));
  }

  template <typename... arg_type>
  inline value call(arg_type&&... args) const {
    return callWithThisRef(nullptr, std::forward<arg_type>(args)...);
  }
  template <typename... arg_type>
  inline value callWithThis(object obj, arg_type&&... args) const {
    return callWithThisRef(obj._ref, std::forward<arg_type>(args)...);
  }

  [[nodiscard]] inline JSObjectRef ref() const { return _ref; }

 private:
  context& _ctx;
  JSObjectRef _ref;

  [[nodiscard]] bool has_property(const details::string_wrapper& name) const;
  [[nodiscard]] inline bool has_property(unsigned int index) const {
    return has_property(std::to_string(index));
  }

  bool remove_property(const details::string_wrapper& name) const;
  inline bool remove_property(unsigned int index) const {
    return remove_property(std::to_string(index));
  }

  [[nodiscard]] value get_property(const details::string_wrapper& name) const;
  [[nodiscard]] value get_property(unsigned int index) const;

  void set_property(const details::string_wrapper& name,
                    const value& val) const;
  void set_property(unsigned int index, const value& val) const;
  template <typename property_type, typename val_type,
            typename = std::enable_if_t<!std::is_same_v<val_type, value>>>
  inline void set_property(const property_type& prop, val_type&& val) const;

  template <typename... arg_type>
  value callWithThisRef(JSObjectRef obj, arg_type&&... args) const;
  template <typename arg_type>
  inline value transform_arg(arg_type&& arg) const;
};

}  // namespace jsc

#endif  // jsc_object_hpp
