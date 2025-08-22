#ifndef ARRAYS_H
#define ARRAYS_H

#include <iostream>
#include <vector>

std::vector<int> slice(const std::vector<int>& v, int start, int end = -1) {
    if (end == -1 || end >= v.size()) end = v.size(); // default to last element + 1
    return std::vector<int>(v.begin() + start, v.begin() + end);
}

#endif // ARRAYS_H