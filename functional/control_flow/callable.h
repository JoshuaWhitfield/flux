#ifndef CALLABLE_H
#define CALLABLE_H

#include <type_traits>
#include <functional>

template <typename F, typename... Args>
inline constexpr bool callable(F&&, Args&&...) {
    return std::is_invocable<F, Args...>::value;
}

// Helper to invoke a callable or return the value as-is
template <typename F, typename... Args>
inline auto invoke_if_callable(F&& f, Args&&... args) {
    if constexpr (std::is_invocable_v<F, Args...>) {
        return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    } else {
        return f; // Not callable, return literal
    }
}

#endif // CALLABLE_H