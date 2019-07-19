#ifndef jsast_utils_hpp
#define jsast_utils_hpp

#include <sstream>
#include <string>
#include <vector>

namespace jsast::utils {

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

inline std::string quoted(const std::string& str) {
  std::ostringstream oss;
  oss << '"';
  for (size_t i{0}; i < str.size(); i++) {
    const auto c{static_cast<uint8_t>(str[i])};
    switch (c) {
      case '\t':
        oss << "\\t";
        break;
      case '\n':
        oss << "\\n";
        break;
      case '\b':
        oss << "\\b";
        break;
      case '\r':
        oss << "\\r";
        break;
      case '\v':
        oss << "\\v";
        break;
      case '\f':
        oss << "\\f";
        break;
      case '"':
        oss << "\\\"";
        break;
      case '\\':
        oss << "\\\\";
        break;
      default:
        oss << c;
        break;
    }
  }
  oss << '"';
  return oss.str();
}

inline std::string backquoted(const std::string& str) {
  std::ostringstream oss;
  for (size_t i{0}; i < str.size(); i++) {
    const auto c{static_cast<uint8_t>(str[i])};
    switch (c) {
      case '\b':
        oss << "\\b";
        break;
      case '\r':
        oss << "\\r";
        break;
      case '\v':
        oss << "\\v";
        break;
      case '\f':
        oss << "\\f";
        break;
      case '`':
        oss << "\\`";
        break;
      case '\\':
        oss << "\\\\";
        break;
      default:
        oss << c;
        break;
    }
  }
  return oss.str();
}

}  // namespace jsast::utils

#endif  // jsast_utils_hpp