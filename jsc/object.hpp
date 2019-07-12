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

  property<details::string_wrapper> operator[](std::string name) const;
  property<unsigned int> operator[](unsigned int index) const;

  bool is_function() const;

  template <typename... arg_types>
  inline value call(arg_types... args) const {
    return callWithThisRef(nullptr, args...);
  }
  template <typename... arg_types>
  inline value callWithThis(object obj, arg_types... args) const {
    return callWithThisRef(obj._ref, args...);
  }

  inline const JSObjectRef ref() const { return _ref; }

 private:
  context& _ctx;
  JSObjectRef _ref;

  bool has_property(details::string_wrapper name) const;
  inline bool has_property(unsigned int index) const {
    return has_property(std::to_string(index));
  }

  bool remove_property(details::string_wrapper name) const;
  inline bool remove_property(unsigned int index) const {
    return remove_property(std::to_string(index));
  }

  value get_property(details::string_wrapper name) const;
  value get_property(unsigned int index) const;

  void set_property(details::string_wrapper name, value val) const;
  void set_property(unsigned int index, value val) const;
  template <typename property_type, typename val_type,
            typename = std::enable_if_t<!std::is_same_v<val_type, value>>>
  inline void set_property(property_type prop, val_type val) const;

  template <typename... arg_types>
  value callWithThisRef(JSObjectRef obj, arg_types... args) const;
};

}  // namespace jsc

#endif  // jsc_object_hpp
