#ifndef FIF_H
#define FIF_H

#include <type_traits>
#include <functional>
#include <tuple>
#include "./callable.h" // invoke_if_callable, callable

// Simplified fif: condition is a plain boolean or callable returning bool
template <typename Cond, typename Do, typename Else>
auto fif(Cond condition, Do do_this, Else else_this) {
    bool cond_result;

    if constexpr (std::is_invocable_v<Cond>) {
        cond_result = condition();  // call if callable
    } else {
        cond_result = static_cast<bool>(condition); // use literal
    }

    if (cond_result) {
        return invoke_if_callable(do_this);
    } else {
        return invoke_if_callable(else_this);
    }
}

#endif // FIF_H
