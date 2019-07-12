#ifndef jsc_string_hpp
#define jsc_string_hpp

#include <JavaScriptCore/JavaScriptCore.h>
#include <string>

namespace jsc::details {

struct string_wrapper {
  inline string_wrapper(std::string str)
      : _ref{JSStringCreateWithUTF8CString(str.data())} {}
  inline string_wrapper(JSStringRef unmanaged) : _ref{unmanaged} {}
  inline ~string_wrapper() { JSStringRelease(_ref); }

  inline string_wrapper(const string_wrapper& str) : _ref{str._ref} {
    JSStringRetain(_ref);
  }
  inline string_wrapper& operator=(const string_wrapper& str) {
    JSStringRelease(_ref);
    _ref = str._ref;
    JSStringRetain(_ref);
    return *this;
  }

  inline std::string get() const {
    auto length = JSStringGetMaximumUTF8CStringSize(_ref);
    auto buffer = std::make_unique<char[]>(length);
    JSStringGetUTF8CString(_ref, buffer.get(), length);
    return buffer.get();
  }
  inline JSStringRef managed_ref() const { return _ref; }

 private:
  JSStringRef _ref;
};

}  // namespace jsc::details

#endif /* jsc_string_hpp */
