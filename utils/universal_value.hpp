#ifndef utils_universal_value_hpp
#define utils_universal_value_hpp

#include <utility>

template <typename value_type>
struct universal_value {
  universal_value(value_type&& value) noexcept(
      noexcept(this->_value = std::forward<value_type>(value)))
      : _value{std::forward<value_type>(value)} {}

  value_type& get() noexcept { return _value; }

 private:
  value_type _value;
};

#endif  // utils_universal_value_hpp
