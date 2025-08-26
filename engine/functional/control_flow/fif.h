#ifndef FIF_H
#define FIF_H

#include <type_traits>
#include <functional>
#include <tuple>

#include "./callable.h" // invoke_if_callable, callable

// fif implementation
template <typename Cond, typename ArgsContainer, typename Do, typename Else>
auto fif(Cond condition, ArgsContainer args, Do do_this, Else else_this) {
    // First, evaluate condition
    auto cond_result = [&] {
        if constexpr (callable(condition, args)) {
            // If args is a tuple, unpack it
            if constexpr (std::is_same_v<ArgsContainer, std::tuple<>>) {
                return std::apply(condition, args);
            } else {
                return condition(args);
            }
        } else {
            return condition; // literal
        }
    }();

    if (cond_result) {
        return invoke_if_callable(do_this);
    } else {
        return invoke_if_callable(else_this);
    }
}

#endif // FIF_H