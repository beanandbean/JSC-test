#ifndef utils_string_hpp
#define utils_string_hpp

#include <sstream>
#include <string>

inline constexpr size_t utf8_length(const std::string& str) noexcept {
  size_t q = 0;
  for (size_t i = 0; i < str.length(); i++, q++) {
    const uint8_t c = str[i];
    if (c >= 0 && c <= 127)
      i += 0;
    else if ((c & 0xE0) == 0xC0)
      i += 1;
    else if ((c & 0xF0) == 0xE0)
      i += 2;
    else if ((c & 0xF8) == 0xF0)
      i += 3;
    else if ((c & 0xFC) == 0xF8)
      i += 4;
    else if ((c & 0xFE) == 0xFC)
      i += 5;
    else
      return 0;  // invalid utf8
  }
  return q;
}

inline std::string quoted(const std::string& str) noexcept {
  std::ostringstream oss;
  oss << '"';
  for (int i = 0; i < str.size(); ++i) {
    const uint8_t c = str[i];
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