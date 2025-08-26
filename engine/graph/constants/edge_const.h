#ifndef EDGECONSTANT_H
#define EDGECONSTANT_H

#include <functional>
#include <vector>

#include "../models/edge.h" // Edge edges

struct EdgeConstant {
    static std::vector<std::reference_wrapper<const Edge>> edge_table() {
        std::vector<std::reference_wrapper<const Edge>> refs;
        for (const auto& e : edges) {
            refs.push_back(std::cref(e));
        }
        return refs;
    }
};

#endif // EDGECONSTANT_H