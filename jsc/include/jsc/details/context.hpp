#ifndef jsc_context_hpp
#define jsc_context_hpp

#include <type_traits>
#include <vector>

#include "object.hpp"
#include "property.hpp"
#include "string.hpp"
#include "value.hpp"

namespace jsc {

struct context_group {
  friend struct context;

  inline context_group() : _ref{JSContextGroupCreate()} {}
  inline ~context_group() { JSContextGroupRelease(_ref); }

  inline context_group(const context_group& group) : _ref{group._ref} {
    JSContextGroupRetain(_ref);
  }
  inline context_group& operator=(const context_group& group) {
    _ref = group._ref;
    JSContextGroupRetain(_ref);
    return *this;
  }

 private:
  JSContextGroupRef _ref;
};

struct context {
  friend struct value;
  friend struct object;

  inline context()
      : _ref{JSGlobalContextCreate(nullptr)}, _exception{undefined()} {}
  inline context(const context_group& group)
      : _ref{JSGlobalContextCreateInGroup(group._ref, nullptr)},
        _exception{undefined()} {}
  inline context(JSGlobalContextRef ref) : _ref{ref}, _exception{undefined()} {
    JSGlobalContextRetain(_ref);
  }
  inline ~context() {
    if (_callback_class != nullptr) {
      JSClassRelease(_callback_class);
    }
    JSGlobalContextRelease(_ref);
  }

  inline context(const context& ctx) : _ref{ctx._ref}, _exception{undefined()} {
    JSGlobalContextRetain(_ref);
  }
  inline context& operator=(const context& ctx) {
    _ref = ctx._ref;
    JSGlobalContextRetain(_ref);
    return *this;
  }

  inline value val(jsc::tag_undefined) {
    return {*this, JSValueMakeUndefined(_ref)};
  }
  inline value val(jsc::tag_null) { return {*this, JSValueMakeNull(_ref)}; }
  inline value val(bool b) { return {*this, JSValueMakeBoolean(_ref, b)}; }
  inline value val(int i) { return val(static_cast<double>(i)); }
  inline value val(double num) { return {*this, JSValueMakeNumber(_ref, num)}; }
  inline value val(const char* str) { return val(std::string{str}); }
  inline value val(std::string str) {
    const auto js_string = details::string_wrapper{str};
    return {*this, JSValueMakeString(_ref, js_string.managed_ref())};
  }
  inline value val(object obj) { return obj; }

  inline value undefined() { return val(jsc::undefined); }
  inline value null() { return val(jsc::null); }

  inline object root() { return {*this, JSContextGetGlobalObject(_ref)}; }
  inline object obj() { return {*this, JSObjectMake(_ref, nullptr, nullptr)}; }

 private:
  using internal_callback_type =
      std::function<JSValueRef(JSContextRef, JSObjectRef, JSObjectRef, size_t,
                               const JSValueRef[], JSValueRef*)>;

  static JSValueRef callback_class_call(JSContextRef ctx, JSObjectRef function,
                                        JSObjectRef this_object,
                                        size_t argument_count,
                                        const JSValueRef arguments[],
                                        JSValueRef* exception);
  static void callback_class_finalize(JSObjectRef function);
  JSClassRef callback_class();

 public:
  template <typename callback_type>
  inline object callable(callback_type callback) {
    const auto callback_func = [&callback](JSContextRef ctx,
                                           JSObjectRef function,
                                           JSObjectRef this_object,
                                           size_t argument_count,
                                           const JSValueRef arguments[],
                                           JSValueRef* exception) {
      context global_context{JSContextGetGlobalContext(ctx)};
      object this_obj{global_context, this_object};
      std::vector<value> vector;
      for (size_t i = 0; i < argument_count; i++) {
        vector.emplace_back(global_context, arguments[i]);
      }

      value result = global_context.undefined();
      value exception_placeholder = global_context.undefined();
      if constexpr (std::is_same_v<decltype(callback(global_context, this_obj,
                                                     vector,
                                                     &exception_placeholder)),
                                   void>) {
        callback(global_context, this_obj, vector, &exception_placeholder);
      } else if constexpr (std::is_same_v<decltype(callback(
                                              global_context, this_obj, vector,
                                              &exception_placeholder)),
                                          value>) {
        result =
            callback(global_context, this_obj, vector, &exception_placeholder);
      } else {
        result = global_context.val(
            callback(global_context, this_obj, vector, &exception_placeholder));
      }
      if (!exception_placeholder.is_undefined()) {
        *exception = exception_placeholder.ref();
      }
      return result.ref();
    };
    return {*this, JSObjectMake(_ref, callback_class(),
                                new internal_callback_type{callback_func})};
  }

  inline bool ok() const { return _exception.is_undefined(); }
  inline const value& get_exception() const { return _exception; }
  inline void clear_exception() { _exception = undefined(); }

  value eval_script(std::string script, std::string source_url = "<anonymous>");

 private:
  JSGlobalContextRef _ref;
  value _exception;
  JSClassRef _callback_class = nullptr;

  inline void set_exception(value exception) {
    if (!exception.is_undefined()) {
      _exception = exception;
    }
  }

  template <typename throwable>
  auto try_throwable(throwable t) {
    JSValueRef exception = JSValueMakeUndefined(_ref);
    if constexpr (std::is_same_v<decltype(t(&exception)), void>) {
      t(&exception);
      set_exception({*this, exception});
    } else {
      const auto result = t(&exception);
      set_exception({*this, exception});
      return result;
    }
  }

  inline void protect(JSValueRef val) const {
    JSGlobalContextRetain(_ref);
    JSValueProtect(_ref, val);
  }
  inline void unprotect(JSValueRef val) const {
    JSValueUnprotect(_ref, val);
    JSGlobalContextRelease(_ref);
  }

 public:
  inline object error(std::string message) {
    // TODO: Add error subclassing
    const auto error_class = root()["Error"].get().to_object();
    const auto message_val = val(message);
    const auto message_ref = message_val.ref();
    return {*this,
            try_throwable([this, &error_class, &message_ref](auto exception) {
              return JSObjectCallAsConstructor(_ref, error_class.ref(), 1,
                                               &message_ref, exception);
            })};
  }
};

}  // namespace jsc

#endif  // jsc_context_hpp
