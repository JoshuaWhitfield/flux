#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <type_traits>
#include <iterator>

// Detect if a type is iterable (has begin/end)
template<typename T, typename = void>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T, std::void_t<
    decltype(std::begin(std::declval<T>())),
    decltype(std::end(std::declval<T>()))
>> : std::true_type {};


// Base case: generic types (not iterable)
template<typename T>
std::enable_if_t<!is_iterable<T>::value, void>
print(const T& value) {
    std::cout << value << "\n";
}

// Special case: std::string (iterable, but print as one unit)
inline void print(const std::string& s) {
    std::cout << s << "\n";
}
inline void print(const char* s) {
    std::cout << s << "\n";
}

// Iterable types (like vector, list, set…)
template<typename T>
std::enable_if_t<is_iterable<T>::value && !std::is_same_v<T, std::string>, void>
print(const T& container) {
    std::cout << "[ ";
    for (const auto& elem : container) {
        std::cout << elem << " ";
    }
    std::cout << "]\n";
}


void inline exit() {
    std::exit(0);
}

#endif // DEBUG_H