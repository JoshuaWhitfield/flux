#ifndef FIM_H
#define FIM_H

#include <functional>
#include "fif.h"        // Your declarative IF wrapper
#include "callable.h"   // callable check

// Variadic Functional Iterator Method (FIM)
template <typename Ret, typename... Args>
inline auto fim(std::function<Ret(Args...)> func, Args... params) {
    return FIF(
        callable(func), // check if function is callable
        [func, params...]() -> Ret {
            return func(params...); // unpack all arguments
        },
        []() -> Ret { return Ret{}; } // fallback if not callable
    );
}

#endif // FIM_H
