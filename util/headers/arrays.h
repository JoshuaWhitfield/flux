#ifndef ARRAYS_H
#define ARRAYS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// Slice function
template<typename T>
inline std::vector<T> slice(const std::vector<T>& v, size_t start, size_t end = SIZE_MAX) {
    if (start >= v.size()) return {};
    if (end == SIZE_MAX || end > v.size()) end = v.size();
    return std::vector<T>(v.begin() + start, v.begin() + end);
}

// Join for vector<string>
inline std::string join(const std::vector<std::string>& vec, const std::string& delimiter = "") {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) oss << delimiter;
    }
    return oss.str();
}

// Join for vector<char>
inline std::string join(const std::vector<char>& vec, const std::string& delimiter = "") {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (!delimiter.empty() && i != vec.size() - 1) oss << delimiter;
    }
    return oss.str();
}

#endif // ARRAYS_H
