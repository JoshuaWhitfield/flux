#ifndef STRINGS_H
#define STRINGS_H

#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

inline std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    result.push_back(str.substr(start));
    return result;
}

inline std::string str_slice(const std::string& s, size_t start, size_t length = std::string::npos) {
    if (start >= s.size()) return "";
    return s.substr(start, std::min(length, s.size() - start));
}

inline bool is_alpha(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isalpha(c);
    });
}

#endif // STRINGS_H