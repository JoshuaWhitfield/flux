#ifndef NODECONSTANT_H
#define NODECONSTANT_H

#include <functional>
#include <vector>
#include "../models/node.h" // Node nodes

template<typename T>
struct NodeConstant {
    // Return a vector of reference_wrappers to const Node<T>
    static std::vector<std::reference_wrapper<const Node<T>>> node_table(const std::vector<Node<T>>& nodes) {
        std::vector<std::reference_wrapper<const Node<T>>> refs;
        for (const auto& e : nodes) {
            refs.push_back(std::cref(e));
        }
        return refs;
    }
};

#endif // NODECONSTANT_H
