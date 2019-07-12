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

  bool is_undefined() const;
  bool is_null() const;
  bool is_boolean() const;
  bool is_number() const;
  bool is_string() const;
  bool is_object() const;

  bool to_boolean() const;
  double to_number() const;
  std::string to_string() const;

  object to_object() const;

  inline const JSValueRef ref() const { return _ref; }

 private:
  context& _ctx;
  JSValueRef _ref;
};

}  // namespace jsc

#endif /* jsc_value_hpp */
