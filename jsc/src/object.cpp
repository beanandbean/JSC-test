#include "object.hpp"

#include "context.hpp"
#include "property.hpp"

namespace jsc {

object::object(context& ctx, JSObjectRef ref) : _ctx{ctx}, _ref{ref} {
  _ctx.protect(_ref);
}

object::~object() { _ctx.unprotect(_ref); }

object::object(const object& obj) : _ctx{obj._ctx}, _ref{obj._ref} {
  _ctx.protect(_ref);
}
object& object::operator=(const object& obj) {
  _ctx.unprotect(_ref);
  _ctx = obj._ctx;
  _ref = obj._ref;
  _ctx.protect(_ref);
  return *this;
}

property<details::string_wrapper> object::operator[](
    const std::string& name) const {
  return {*this, name};
}

property<unsigned int> object::operator[](unsigned int index) const {
  return {*this, index};
}

bool object::is_function() const { return JSObjectIsFunction(_ctx._ref, _ref); }

bool object::has_property(const details::string_wrapper& name) const {
  return JSObjectHasProperty(_ctx._ref, _ref, name.managed_ref());
}

bool object::remove_property(const details::string_wrapper& name) const {
  return _ctx.try_throwable([this, &name](auto exception) {
    return JSObjectDeleteProperty(_ctx._ref, _ref, name.managed_ref(),
                                  exception);
  });
}

value object::get_property(const details::string_wrapper& name) const {
  return {_ctx, _ctx.try_throwable([this, &name](auto exception) {
            return JSObjectGetProperty(_ctx._ref, _ref, name.managed_ref(),
                                       exception);
          })};
}

value object::get_property(unsigned int index) const {
  return {_ctx, _ctx.try_throwable([this, &index](auto exception) {
            return JSObjectGetPropertyAtIndex(_ctx._ref, _ref, index,
                                              exception);
          })};
}

void object::set_property(const details::string_wrapper& name,
                          const value& val) const {
  _ctx.try_throwable([this, &name, &val](auto exception) {
    JSObjectSetProperty(_ctx._ref, _ref, name.managed_ref(), val.ref(),
                        kJSPropertyAttributeNone, exception);
  });
}
void object::set_property(unsigned int index, const value& val) const {
  _ctx.try_throwable([this, &index, &val](auto exception) {
    JSObjectSetPropertyAtIndex(_ctx._ref, _ref, index, val.ref(), exception);
  });
}

}  // namespace jsc
