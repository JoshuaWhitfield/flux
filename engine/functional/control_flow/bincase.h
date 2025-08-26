#ifndef BINCASE_H
#define BINCASE_H

#include "callable.h" // invoke_if_callable callable
#include <tuple>
#include <type_traits>
#include <utility>

// Helper to invoke a callable or return the value as-is
template <typename F, typename... Args>
auto invoke_if_callable(F&& f, Args&&... args) {
    if constexpr (std::is_invocable_v<F, Args...>) {
        return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    } else {
        return f; // Not callable, return literal
    }
}

// Bincase implementation
template <typename Cond, typename OnSuccess, typename OnFailure, typename Params = std::tuple<>>
auto bincase(Cond conditional, OnSuccess onSuccess, OnFailure onFailure, Params params = {}) {

    auto cond_result = [&] {
        if constexpr (callable(conditional)) {
            if constexpr (std::tuple_size_v<Params> > 0) {
                return std::apply(conditional, params); // unpack tuple
            } else {
                return conditional();
            }
        } else {
            return conditional; // literal value
        }
    }();

    if (cond_result) {
        return invoke_if_callable(onSuccess);
    } else {
        return invoke_if_callable(onFailure);
    }
}

#endif // BINCASE_H