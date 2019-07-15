#ifndef utils_parameter_pack_hpp
#define utils_parameter_pack_hpp

#include <cstddef>

template <typename... types>
struct parameter_pack_counter {
  static constexpr size_t count{0};
};

template <typename... types>
constexpr size_t parameter_pack_count{parameter_pack_counter<types...>::count};

template <typename t0, typename... types>
struct parameter_pack_counter<t0, types...> {
  static constexpr size_t count{1 + parameter_pack_count<types...>};
};

#endif  // utils_parameter_pack_hpp
