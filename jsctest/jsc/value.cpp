//
//  value.cpp
//  jsc
//
//  Created by Wang Shuwei on 2019/7/4.
//  Copyright © 2019 BeanandBean. All rights reserved.
//

#include "value.hpp"

#include "object.hpp"
#include "context.hpp"
#include "string.hpp"

namespace jsc {
    
value::value(context& ctx, JSValueRef ref) : _ctx{ctx}, _ref{ref} {
    _ctx.protect(_ref);
}
value::~value() {
    _ctx.unprotect(_ref);
}
    
value::value(const value& val) : _ctx{val._ctx}, _ref{val._ref} {
    _ctx.protect(_ref);
}
value& value::operator=(const value& val) {
    _ctx.unprotect(_ref);
    _ctx = val._ctx;
    _ref = val._ref;
    _ctx.protect(_ref);
    return *this;
}
    
bool value::is_undefined() const {
    return JSValueIsUndefined(_ctx._ref, _ref);
}
bool value::is_null() const {
    return JSValueIsNull(_ctx._ref, _ref);
}
bool value::is_boolean() const {
    return JSValueIsBoolean(_ctx._ref, _ref);
}
bool value::is_number() const {
    return JSValueIsNumber(_ctx._ref, _ref);
}
bool value::is_string() const {
    return JSValueIsString(_ctx._ref, _ref);
}
bool value::is_object() const {
    return JSValueIsObject(_ctx._ref, _ref);
}
    
bool value::to_boolean() const {
    return JSValueToBoolean(_ctx._ref, _ref);
}
double value::to_number() const {
    return _ctx.try_throwable([this](auto exception) {
        return JSValueToNumber(_ctx._ref, _ref, exception);
    });
}
std::string value::to_string() const {
    return _ctx.try_throwable([this](auto exception) {
        return details::string_wrapper{JSValueToStringCopy(_ctx._ref, _ref, exception)}.get();
    });
}
    
object value::to_object() const {
    return {_ctx, _ctx.try_throwable([this](auto exception) {
        return JSValueToObject(_ctx._ref, _ref, exception);
    })};
}
    
} // namespace jsc
