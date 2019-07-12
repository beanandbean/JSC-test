#ifndef jscodegen_generator_hpp
#define jscodegen_generator_hpp

#include <sstream>

#include "../utils/string.hpp"
#include "source_loc.hpp"

namespace jscodegen {

struct generator {
  struct {
    std::string indent = "  ";
    std::string line_end = "\n";
  } config;

  template <typename number_type>
  inline source_range write_number_literal(number_type number) {
    return with_range([this, &number]() { write_raw(std::to_string(number)); });
  }

  inline source_range write_raw_literal(std::string literal) {
    return with_range([this, &literal]() { write_raw(literal); });
  }

  inline source_range write_string_literal(std::string str) {
    return with_range([this, &str]() { write_raw(quoted(str)); });
  }

  inline std::string str() const { return _buffer.str(); }

 private:
  std::ostringstream _buffer;
  source_loc _loc = {1, 1};
  size_t _indent_level;

  template <typename callable_type>
  inline source_range with_range(callable_type callable) const {
    const source_loc start = _loc;
    callable();
    return {start, _loc};
  }

  inline void write_raw(std::string str) {
    _buffer << str;
    _loc.column += utf8_length(str);
  }

  inline void write_new_line() {
    _buffer << config.line_end;
    _loc = {_loc.line + 1, 1};
  }
};

}  // namespace jscodegen

#endif  // jscodegen_generator_hpp