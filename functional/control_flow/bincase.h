#ifndef BINCASE_H
#define BINCASE_H

#include "callable.h" // invoke_if_callable callable
#include <tuple>
#include <type_traits>
#include <utility>

// Bincase implementation
template <typename Cond, typename OnSuccess, typename OnFailure>
inline auto bincase(Cond conditional, OnSuccess onSuccess, OnFailure onFailure) {

    auto cond_result = [&] {
        if constexpr (callable(conditional)) {
            return conditional();
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