#ifndef jsc_object_inc
#define jsc_object_inc

#include <algorithm>
#include <array>

#include "context.hpp"

namespace jsc {

namespace utils {

template <typename... types>
struct parameter_pack_counter {
  static constexpr size_t count{0};
};

template <typename... types>
constexpr size_t parameter_pack_count{parameter_pack_counter<types...>::count};

template <typename t0, typename... types>
struct parameter_pack_counter<t0, types...> {
  static constexpr size_t count{1 + parameter_pack_count<types...>};
};

}  // namespace utils

template <typename property_type, typename val_type, typename>
inline void object::set_property(const property_type& prop,
                                 val_type&& val) const {
  set_property(prop, _ctx.val(std::forward<val_type>(val)));
}

template <typename... arg_type>
value object::callWithThisRef(JSObjectRef obj, arg_type&&... args) const {
  if constexpr (utils::parameter_pack_count<arg_type...> == 0) {
    return {_ctx, _ctx.try_throwable([this, &obj](auto exception) {
              return JSObjectCallAsFunction(_ctx._ref, _ref, obj, 0, nullptr,
                                            exception);
            })};
  } else {
    const std::array<value, utils::parameter_pack_count<arg_type...>> arg_list{
        transform_arg(std::forward<arg_type>(args))...};
    std::array<JSValueRef, arg_list.size()> ref_list;
    std::transform(arg_list.begin(), arg_list.end(), ref_list.begin(),
                   [](const auto val) { return val.ref(); });

    return {_ctx, _ctx.try_throwable([this, &obj, &ref_list](auto exception) {
              return JSObjectCallAsFunction(_ctx._ref, _ref, obj,
                                            ref_list.size(), ref_list.data(),
                                            exception);
            })};
  }
}

template <typename arg_type>
inline value object::transform_arg(arg_type&& arg) const {
  if constexpr (std::is_same_v<std::decay_t<arg_type>, value>) {
    return std::forward<arg_type>(arg);
  } else {
    return _ctx.val(std::forward<arg_type>(arg));
  }
}

}  // namespace jsc

#endif  // jsc_object_inc
