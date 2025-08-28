#ifndef FIF_H
#define FIF_H
#include <type_traits>
#include <functional>
#include <tuple>
#include "./callable.h" // invoke_if_callable, callable

// Simplified fif: condition can be bool, callable returning bool, or callable returning void
template <typename Cond, typename Do, typename Else>
inline auto fif(Cond condition, Do do_this, Else else_this) {
    bool cond_result;
    
    if constexpr (std::is_invocable_v<Cond>) {
        if constexpr (std::is_same_v<std::invoke_result_t<Cond>, void>) {
            // Condition returns void - treat as always true
            condition();
            cond_result = true;
        } else {
            // Condition returns something - convert to bool
            cond_result = static_cast<bool>(condition());
        }
    } else {
        // Condition is a literal value
        cond_result = static_cast<bool>(condition);
    }
    
    if (cond_result) {
        return invoke_if_callable(do_this);
    } else {
        return invoke_if_callable(else_this);
    }
}

#endif // FIF_H