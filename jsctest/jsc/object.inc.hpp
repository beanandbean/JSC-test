//
//  object.inc
//  jsc
//
//  Created by Wang Shuwei on 2019/7/4.
//  Copyright © 2019 BeanandBean. All rights reserved.
//

#ifndef jsc_object_inc
#define jsc_object_inc

#include <algorithm>
#include <array>

#include "context.hpp"
#include "utils.hpp"

namespace jsc {

template <typename property_type, typename val_type, typename>
inline void object::set_property(property_type prop, val_type val) const {
    set_property(prop, _ctx.val(val));
}

template <typename ... arg_types>
value object::callWithThisRef(JSObjectRef obj, arg_types ... args) const {
    if constexpr (parameter_pack_count<arg_types ...> == 0) {
        return {_ctx, _ctx.try_throwable([this, &obj](auto exception) {
            return JSObjectCallAsFunction(_ctx._ref, _ref, obj, 0, nullptr, exception);
        })};
    } else {
        const auto arg_map = [this](auto arg) {
            if constexpr (std::is_same_v<decltype(arg), value>) {
                return arg;
            } else {
                return _ctx.val(arg);
            }
        };
        std::array<value, parameter_pack_count<arg_types ...>> arg_list{arg_map(args) ...};
        std::array<JSValueRef, arg_list.size()> ref_list;
        std::transform(arg_list.begin(), arg_list.end(), ref_list.begin(), [](auto val) {
            return val.ref();
        });
        
        return {_ctx, _ctx.try_throwable([this, &obj, &ref_list](auto exception) {
            return JSObjectCallAsFunction(_ctx._ref, _ref, obj, ref_list.size(), ref_list.data(), exception);
        })};
    }
}
    
}

#endif /* jsc_object_inc */
