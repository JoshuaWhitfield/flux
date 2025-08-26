#ifndef COMPARISONS_H
#define COMPARISONS_H

#include <optional>
#include <string>
#include <type_traits>

// feq: functional equality
// returns left value if equal, std::nullopt if not
template<typename T, typename U>
inline std::optional<T> feq(const T& left, const U& right) {
    if (left == right) {
        return left;
    }
    return std::nullopt;
}

// fneq: functional not-equal
// returns true if not equal, false otherwise
template<typename T, typename U>
inline bool fneq(const T& left, const U& right) {
    return left != right;
}

// fgeq: functional greater-or-equal
// returns whichever parameter is greater by "size" if available
// if equal, returns left
template<typename T, typename U>
inline auto fgeq(const T& left, const U& right) {
    if constexpr (requires { left.size(); right.size(); }) {
        // Compare by size if both types support .size()
        if (left.size() > right.size()) return left;
        if (left.size() < right.size()) return right;
        return left; // equal sizes → return left
    } else {
        // Fallback: compare by operator>=
        if (left >= right) return left;
        return right;
    }
}

// fleq: functional less-or-equal
// returns left if <= right, std::nullopt otherwise
template<typename T, typename U>
inline std::optional<T> fleq(const T& left, const U& right) {
    if (left <= right) {
        return left;
    }
    return std::nullopt;
}

// fgt: functional greater-than
// returns true if left > right, else false
template<typename T, typename U>
inline bool fgt(const T& left, const U& right) {
    return left > right;
}

// flt: functional less-than
// returns true if left < right, else false
template<typename T, typename U>
inline bool flt(const T& left, const U& right) {
    return left < right;
}

// fpeq: functional plus-equal (+=)
// returns the result of left + right (like += but functional style)
// does NOT mutate left — pure function
template<typename T, typename U>
inline auto fpeq(const T& left, const U& right) {
    return left + right;
}

#include <type_traits>
#include <optional>

// fseq: functional subtract-equal (-=)
// returns left - right (pure, non-mutating)
template<typename T, typename U>
inline auto fseq(const T& left, const U& right) {
    return left - right;
}

// fmeq: functional multiply-equal (*=)
// returns left * right (pure, non-mutating)
template<typename T, typename U>
inline auto fmeq(const T& left, const U& right) {
    return left * right;
}

// fdeq: functional divide-equal (/=)
// returns left / right (pure, non-mutating)
// handles division by zero by returning std::nullopt
template<typename T, typename U>
inline auto fdeq(const T& left, const U& right) -> std::optional<decltype(left / right)> {
    if (right == 0) return std::nullopt;
    return left / right;
}

// fmdeq: functional modulo-equal (%=)
// returns left % right (pure, non-mutating)
// only enabled for integral types
template<typename T, typename U>
inline auto fmdeq(const T& left, const U& right) -> std::optional<decltype(left % right)> {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
        if (right == 0) return std::nullopt;
        return left % right;
    } else {
        return std::nullopt; // modulo not defined for non-integral
    }
}

// fxeq: functional xor-equal (^=)
// returns left ^ right (pure, non-mutating)
// only enabled for integral types
template<typename T, typename U>
inline auto fxeq(const T& left, const U& right) -> std::optional<decltype(left ^ right)> {
    if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
        return left ^ right;
    } else {
        return std::nullopt;
    }
}

#endif // COMPARISONS_H