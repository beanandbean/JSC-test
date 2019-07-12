#include "context.hpp"

namespace jsc {

value context::eval_script(std::string script, std::string source_url) {
  const details::string_wrapper js_url{source_url};
  const details::string_wrapper js_script{script};
  return {*this, try_throwable([this, &js_script, &js_url](auto exception) {
            return JSEvaluateScript(_ref, js_script.managed_ref(), nullptr,
                                    js_url.managed_ref(), 0, exception);
          })};
}

JSValueRef context::callback_class_call(JSContextRef ctx, JSObjectRef function,
                                        JSObjectRef this_object,
                                        size_t argument_count,
                                        const JSValueRef arguments[],
                                        JSValueRef* exception) {
  auto callback =
      static_cast<internal_callback_type*>(JSObjectGetPrivate(function));
  return (*callback)(ctx, function, this_object, argument_count, arguments,
                     exception);
}

void context::callback_class_finalize(JSObjectRef function) {
  auto callback =
      static_cast<internal_callback_type*>(JSObjectGetPrivate(function));
  delete callback;
}

JSClassRef context::callback_class() {
  if (_callback_class == nullptr) {
    JSClassDefinition def = kJSClassDefinitionEmpty;
    def.className = "NativeCallback";
    def.attributes = kJSClassAttributeNone;
    def.callAsFunction = callback_class_call;
    def.finalize = callback_class_finalize;
    _callback_class = JSClassCreate(&def);
  }
  return _callback_class;
}

}  // namespace jsc
