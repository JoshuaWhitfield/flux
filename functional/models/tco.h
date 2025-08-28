#ifndef TAILCALL_H
#define TAILCALL_H

#include <functional>  // for std::function (callbacks, TailCall lambdas)

template <typename T>
struct TailCall {
    std::function<T()> func;
    TailCall(std::function<T()> f) : func(f) {}
    T run() { return func(); }
};


#endif // TAILCALL_H