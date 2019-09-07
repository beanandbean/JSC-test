#ifndef jsc_value_hpp
#define jsc_value_hpp

#include <JavaScriptCore/JavaScriptCore.h>
#include <string>

namespace jsc {

struct tag_undefined {};
struct tag_null {};

static constexpr tag_undefined undefined{};
static constexpr tag_null null{};

struct context;
struct object;

struct value {
  value(context& ctx, JSValueRef ref);
  ~value();

  value(const value& val);
  value& operator=(const value& val);

  [[nodiscard]] bool is_undefined() const;
  [[nodiscard]] bool is_null() const;
  [[nodiscard]] bool is_boolean() const;
  [[nodiscard]] bool is_number() const;
  [[nodiscard]] bool is_string() const;
  [[nodiscard]] bool is_object() const;

  [[nodiscard]] bool to_boolean() const;
  [[nodiscard]] double to_number() const;
  [[nodiscard]] std::string to_string() const;

  [[nodiscard]] object to_object() const;

  [[nodiscard]] inline JSValueRef ref() const { return _ref; }

 private:
  context* _ctx;
  JSValueRef _ref;
};

}  // namespace jsc

#endif  // jsc_value_hpp
