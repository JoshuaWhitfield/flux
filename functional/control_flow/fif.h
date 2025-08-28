#ifndef FIF_H
#define FIF_H
#include <type_traits>
#include <functional>
#include "./callable.h" // invoke_if_callable, callable

// fif specialized for returning bool
template <typename Cond, typename Do, typename Else>
inline bool fif(Cond condition, Do do_this, Else else_this) {
    bool cond_result;

    if constexpr (std::is_invocable_v<Cond>) {
        if constexpr (std::is_same_v<std::invoke_result_t<Cond>, void>) {
            condition();
            cond_result = true; // treat void as true
        } else {
            cond_result = static_cast<bool>(condition());
        }
    } else {
        cond_result = static_cast<bool>(condition);
    }

    if (cond_result) {
        if constexpr (std::is_invocable_v<Do>) {
            if constexpr (std::is_same_v<std::invoke_result_t<Do>, void>) {
                do_this();
                return true; // default true for void
            } else {
                return static_cast<bool>(do_this());
            }
        } else {
            return static_cast<bool>(do_this);
        }
    } else {
        if constexpr (std::is_invocable_v<Else>) {
            if constexpr (std::is_same_v<std::invoke_result_t<Else>, void>) {
                else_this();
                return false; // default false for void
            } else {
                return static_cast<bool>(else_this());
            }
        } else {
            return static_cast<bool>(else_this);
        }
    }
}
#endif // FIF_H
