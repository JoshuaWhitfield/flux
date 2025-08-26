#ifndef TAILCALL_H
#define TAILCALL_H

#include <functional>  // for std::function (callbacks, TailCall lambdas)

template <typename Ret>
struct TailCall {
    std::function<std::variant<std::function<TailCall<Ret>()>, Ret>()> next;

    TailCall(std::function<std::variant<std::function<TailCall<Ret>()>, Ret>()> fn) : next(fn) {}

    Ret run() {
        auto current = *this;
        std::variant<std::function<TailCall<Ret>()>, Ret> result;

        // keep calling next functions recursively but encoded as "next" function objects
        while (true) { 
            result = current.next();
            if (std::holds_alternative<Ret>(result)) return std::get<Ret>(result);
            current = std::get<std::function<TailCall<Ret>()>>(result)();
        }
    }
};


#endif // TAILCALL_H