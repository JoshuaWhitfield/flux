#ifndef NODE_H
#define NODE_H

#include <optional>
#include <functional>
#include <vector>
#include <string>
#include <cstdint>

#include "./edge.h"

template<typename T>
struct Node {
    uint32_t id;
    std::string type;
    T object; // custom objects that describe what the graph node is
    std::vector<size_t> edge_idxs;

    // Constructor
    Node(const uint32_t& _id, const std::string& _type, T object, const std::vector<size_t>& _edge_idxs = {})
        : id(_id), type(_type), object(object), edge_idxs(_edge_idxs) {}

    // Getters
    const std::vector<size_t>& edge_indices() const { return edge_idxs; }
    const std::string& get_type() const { return type; }
    uint32_t get_id() const { return id; }
    const T& get_value() const { return value; }

    // Mutators
    void append_edge(size_t _edge) { edge_idxs.push_back(_edge); }
    bool is_empty() const { return edge_idxs.empty(); }

    // Reference-friendly optional accessors
    std::optional<size_t> last_edge() const {
        if (!edge_idxs.empty()) return edge_idxs.back();
        return std::nullopt;
    }

    std::optional<size_t> first_edge() const {
        if (!edge_idxs.empty()) return edge_idxs.front();
        return std::nullopt;
    }

    std::optional<size_t> get_edge(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= edge_idxs.size()) return std::nullopt;
        return edge_idxs[index];
    }
};

inline std::vector<Node<int>> nodes;

#endif // NODE_H
