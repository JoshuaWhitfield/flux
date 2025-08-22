#ifndef ARRAYS_H
#define ARRAYS_H

#include <iostream>
#include <sstream>
#include <vector>

std::vector<int> slice(const std::vector<int>& v, int start, int end = -1) {
    if (end == -1 || end >= v.size()) end = v.size(); // default to last element + 1
    return std::vector<int>(v.begin() + start, v.begin() + end);
}

inline std::string join(const std::vector<std::string>& vec, const std::string& delimiter = "") {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) {
            oss << delimiter;
        }
    }
    return oss.str();
}

#endif // ARRAYS_H