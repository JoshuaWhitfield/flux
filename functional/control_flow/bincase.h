#ifndef BINCASE_H
#define BINCASE_H

#include "callable.h" // invoke_if_callable callable
#include "fif.h"        // fif functional if
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

// Simplified fif: condition can be bool, callable returning bool, or callable returning void
template <typename Cond, typename OnSuccess, typename OnFailure>
inline auto bincase(Cond conditional, OnSuccess onSuccess, OnFailure onFailure) {
    // Evaluate conditional: call if callable, otherwise use literal
    auto cond_result = [&]() {
        if constexpr (callable(conditional)) {
            return conditional();  // only call if callable
        } else {
            return conditional;    // just a literal
        }
    }();

    // Now cond_result is true/false, so fif works correctly
    return fif(
        [&] { return cond_result; },        // condition lambda
        [&] { return invoke_if_callable(onSuccess); }, // true branch
        [&] { return invoke_if_callable(onFailure); }  // false branch
    );
}

#endif // BINCASE_H