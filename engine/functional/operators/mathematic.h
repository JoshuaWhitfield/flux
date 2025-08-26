#ifndef MATHEMATIC_H
#define MATHEMATIC_H

#include <optional>
#include <type_traits>

// fmin: subtraction (-)
template<typename T, typename U>
auto fmin(const T& left, const U& right) {
    return left - right;
}

// fadd: addition (+)
template<typename T, typename U>
auto fadd(const T& left, const U& right) {
    return left + right;
}

// fmul: multiplication (*)
template<typename T, typename U>
auto fmul(const T& left, const U& right) {
    return left * right;
}

// fdiv: division (/)
// handles division by zero with std::nullopt
template<typename T, typename U>
auto fdiv(const T& left, const U& right) -> std::optional<decltype(left / right)> {
    if (right == 0) return std::nullopt;
    return left / right;
}

// fmod: modulo (%)
// only for integral types
template<typename T, typename U>
auto fmod(const T& left, const U& right) -> std::optional<decltype(left % right)> {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
        if (right == 0) return std::nullopt;
        return left % right;
    } else {
        return std::nullopt;
    }
}

// fexp: exponentiation (^)
// here using std::pow for arithmetic, or ^ for integral (bitwise XOR)
// design choice: I’ll assume math exponentiation
#include <cmath>
template<typename T, typename U>
auto fexp(const T& base, const U& exponent) {
    return std::pow(base, exponent);
}

// finc: increment (++)
template<typename T>
auto finc(const T& value) {
    return value + static_cast<T>(1);
}

// fdec: decrement (--)
template<typename T>
auto fdec(const T& value) {
    return value - static_cast<T>(1);
}

#endif // MATHEMATIC_H