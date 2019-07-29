#include <utility>

#include "context.hpp"

namespace jsc {

value context::eval_script(const std::string& script,
                           const std::string& source_url) {
  return {*this,
          try_throwable(
              [this, js_script{details::string_wrapper{script}},
               js_url{details::string_wrapper{source_url}}](auto exception) {
                return JSEvaluateScript(_ref, js_script.managed_ref(), nullptr,
                                        js_url.managed_ref(), 0, exception);
              })};
}

JSValueRef context::callback_class_call(JSContextRef ctx, JSObjectRef function,
                                        JSObjectRef this_object,
                                        size_t argument_count,
                                        const JSValueRef arguments[],
                                        JSValueRef* exception) {
  auto callback{
      static_cast<internal_callback_type*>(JSObjectGetPrivate(function))};
  return (*callback)(ctx, function, this_object, argument_count, arguments,
                     exception);
}

void context::callback_class_finalize(JSObjectRef function) {
  delete static_cast<internal_callback_type*>(JSObjectGetPrivate(function));
}

}  // namespace jsc
