#include <vector>
#include <tuple>
#include <string>
#include <string_view>
#include <type_traits>

//----------------------------
// FCAT
//----------------------------

// Generic template: fallback for types supporting operator+
template <typename T1, typename T2>
auto fcat(const T1& a, const T2& b) -> decltype(a + b) {
    return a + b;
}

// Specialization for std::vector
template <typename T>
std::vector<T> fcat(const std::vector<T>& a, const std::vector<T>& b) {
    std::vector<T> result = a;
    result.insert(result.end(), b.begin(), b.end());
    return result;
}

// Specialization for std::tuple
template <typename... T1, typename... T2>
auto fcat(const std::tuple<T1...>& a, const std::tuple<T2...>& b) {
    return std::tuple_cat(a, b);
}

// Specialization for string_view (returns std::string)
inline std::string fcat(std::string_view a, std::string_view b) {
    return std::string(a) + std::string(b);
}

// Specialization for mixed string types
template <typename T>
auto fcat(const std::string& a, const T& b) {
    return a + b;
}

template <typename T>
auto fcat(const T& a, const std::string& b) {
    return a + b;
}
