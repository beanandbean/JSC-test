#ifndef utils_move_vector_hpp
#define utils_move_vector_hpp

#include <memory>
#include <vector>

template <typename elem_type>
struct move_vector : std::vector<elem_type> {
  template <typename... arg_type>
  move_vector(arg_type&&... args) {
    push_all(std::forward<arg_type>(args)...);
  }

 private:
  inline void push_all() noexcept {}
  template <typename... arg_type>
  inline void push_all(elem_type&& n, arg_type&&... args) {
    this->push_back(std::move(n));
    push_all(std::forward<arg_type>(args)...);
  }
};

#endif  // utils_move_vector_hpp