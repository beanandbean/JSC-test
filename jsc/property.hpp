#ifndef jsc_property_hpp
#define jsc_property_hpp

#include <type_traits>

#include "object.hpp"

namespace jsc {

template <typename property_type>
struct property {
  inline property(const object& obj, property_type prop)
      : _obj{obj}, _prop{prop} {}

  inline property(const property<property_type>& prop)
      : _obj{prop._obj}, _prop{prop._prop} {}

  inline property& operator=(const property<property_type>& prop) {
    _obj = prop._obj;
    _prop = prop._prop;
    return *this;
  }

  inline bool exists() const { return _obj.has_property(_prop); }

  inline bool remove() const { return _obj.remove_property(_prop); }

  inline value get() const { return _obj.get_property(_prop); }

  template <typename val_type>
  inline void set(val_type val) const {
    _obj.set_property(_prop, val);
  }

  template <typename val_type, typename = std::enable_if_t<!std::is_same_v<
                                   val_type, property<property_type>>>>
  inline val_type operator=(val_type val) {
    set(val);
    return val;
  }

 private:
  object _obj;
  property_type _prop;
};

}  // namespace jsc

#endif  // jsc_property_hpp
