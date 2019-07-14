#include "generator.hpp"

namespace jscodegen {

void generator::write_raw(std::string str) {
  _buffer << str;

  // UTF-8 line counting
  for (size_t i = 0; i < str.length(); i++) {
    const uint8_t c = str[i];
    if ((c & 0xE0) == 0xC0) {
      i += 1;
    } else if ((c & 0xF0) == 0xE0) {
      i += 2;
    } else if ((c & 0xF8) == 0xF0) {
      i += 3;
    } else if ((c & 0xFC) == 0xF8) {
      i += 4;
    } else if ((c & 0xFE) == 0xFC) {
      i += 5;
    }

    if (c == '\n') {
      _loc.line++;
      _loc.column = 1;
    } else {
      _loc.column++;
    }
  }
}

}  // namespace jscodegen