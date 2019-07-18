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
inline void object::set_property(property_type prop, val_type val) const {
  set_property(prop, _ctx.val(val));
}

template <typename... arg_type>
value object::callWithThisRef(JSObjectRef obj, arg_type... args) const {
  if constexpr (utils::parameter_pack_count<arg_type...> == 0) {
    return {_ctx, _ctx.try_throwable([this, &obj](auto exception) {
              return JSObjectCallAsFunction(_ctx._ref, _ref, obj, 0, nullptr,
                                            exception);
            })};
  } else {
    const auto arg_map{[this](auto arg) {
      if constexpr (std::is_same_v<decltype(arg), value>) {
        return arg;
      } else {
        return _ctx.val(arg);
      }
    }};
    std::array<value, utils::parameter_pack_count<arg_type...>> arg_list{
        arg_map(args)...};
    std::array<JSValueRef, arg_list.size()> ref_list;
    std::transform(arg_list.begin(), arg_list.end(), ref_list.begin(),
                   [](auto val) { return val.ref(); });

    return {_ctx, _ctx.try_throwable([this, &obj, &ref_list](auto exception) {
              return JSObjectCallAsFunction(_ctx._ref, _ref, obj,
                                            ref_list.size(), ref_list.data(),
                                            exception);
            })};
  }
}

}  // namespace jsc

#endif  // jsc_object_inc
