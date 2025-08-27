#ifndef SCOPE_H
#define SCOPE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <cstdint>
#include "./node.h"
#include "./edge.h"

// Forward declaration for Node
template <typename T>
struct Node;

class Scope {
public:
    uint32_t id;                               // unique id for this scope
    std::optional<size_t> parent_scope_idx;    // index of parent scope in graph
    std::vector<size_t> child_scope_indices;   // indices of child scopes
    std::unordered_map<std::string, size_t> symbol_table; // maps variable names to node indices
    std::vector<size_t> node_indices;          // nodes that belong directly to this scope

    // Constructor
    Scope(uint32_t _id, std::optional<size_t> _parent = std::nullopt)
        : id(_id), parent_scope_idx(_parent) {}

    // Add a child scope
    void add_child_scope(size_t child_idx) {
        child_scope_indices.push_back(child_idx);
    }

    // Add a node to this scope
    void add_node(size_t node_idx) {
        node_indices.push_back(node_idx);
    }

    // Register a symbol (variable name)
    void register_symbol(const std::string& name, size_t node_idx) {
        symbol_table[name] = node_idx;
        add_node(node_idx);
    }

    // Lookup a symbol in this scope (does not check parents)
    std::optional<size_t> lookup_local(const std::string& name) const {
        auto it = symbol_table.find(name);
        if (it != symbol_table.end()) return it->second;
        return std::nullopt;
    }

    // Lookup a symbol recursively in parent scopes
    std::optional<size_t> lookup(const std::string& name, const std::vector<Scope>& all_scopes) const {
        auto local = lookup_local(name);
        if (local) return local;
        if (parent_scope_idx) {
            return all_scopes[*parent_scope_idx].lookup(name, all_scopes);
        }
        return std::nullopt;
    }

    // Accessors
    const std::vector<size_t>& get_nodes() const { return node_indices; }
    const std::vector<size_t>& get_children() const { return child_scope_indices; }
    std::optional<size_t> get_parent() const { return parent_scope_idx; }
};

#endif // SCOPE_H
