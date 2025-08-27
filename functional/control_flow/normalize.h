// normalize.h
#ifndef NORMALIZE_H
#define NORMALIZE_H

#include <optional>
#include <type_traits>

// Trait to detect if a type is std::optional
template <typename>
struct is_std_optional : std::false_type {};

template <typename U>
struct is_std_optional<std::optional<U>> : std::true_type {};

template <typename T>
inline constexpr bool is_std_optional_v = is_std_optional<T>::value;

// Generic normalize to std::optional<T>
template <typename T>
inline std::optional<T> normalize(const T& value) {
    if constexpr (is_std_optional_v<T>) {
        return value; // already optional
    } 
    else if constexpr (std::is_same_v<T, bool>) {
        return value ? std::optional<T>(true) : std::nullopt;
    } 
    else {
        return std::optional<T>(value);
    }
}

// Overload for pointers
template <typename T>
inline std::optional<T*> normalize(T* ptr) {
    return ptr ? std::optional<T*>(ptr) : std::nullopt;
}

#endif // NORMALIZE_H
