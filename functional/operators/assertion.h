#ifndef FUNCTIONAL_DECLARATIVES_H
#define FUNCTIONAL_DECLARATIVES_H

#include <functional>
#include <type_traits>
#include <string>
#include <sstream>
#include <iostream>
#include <variant>
#include <vector>

//----------------------------
// ASSERT
//----------------------------
// Executes conditionFn, returns the value if true, otherwise throws runtime error
template <typename T>
T fassert(std::function<bool()> conditionFn, T returnValue) {
    if (conditionFn()) {
        return returnValue;
    } else {
        throw std::runtime_error("Assertion failed");
    }
}

//----------------------------
// CALLABLE
//----------------------------
// Checks if a function can be invoked with given args
template <typename F, typename... Args>
bool fcallable(F&& func, Args&&... args) {
    return std::is_invocable<F, Args...>::value;
}

//----------------------------
// TYPE
//----------------------------
// Returns type name of value as string
template <typename T>
std::string ftype(const T&) {
    return typeid(T).name();
}

#endif // FUNCTIONAL_DECLARATIVES_H
