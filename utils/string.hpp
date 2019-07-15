#ifndef utils_string_hpp
#define utils_string_hpp

#include <sstream>
#include <string>

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
      case '\a':
        oss << "\\a";
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

#endif  // utils_string_hpp
