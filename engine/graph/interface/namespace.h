#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include "node.h"

template <typename T>
struct Namespace {
    uint32_t id;                                 // unique id
    std::string name;                            // namespace name
    std::vector<size_t> node_indices;            // nodes that belong to this namespace
    std::unordered_map<std::string, size_t> symbol_table; // symbol -> node index
    std::optional<size_t> parent_namespace_idx; // allow nested namespaces
    std::vector<size_t> child_namespaces;       // nested namespaces

    Namespace(uint32_t _id, const std::string& _name, std::optional<size_t> _parent = std::nullopt)
        : id(_id), name(_name), parent_namespace_idx(_parent) {}

    void add_node(size_t node_idx) { node_indices.push_back(node_idx); }
    void add_child_namespace(size_t ns_idx) { child_namespaces.push_back(ns_idx); }
    void register_symbol(const std::string& name, size_t node_idx) { 
        symbol_table[name] = node_idx; 
        add_node(node_idx);
    }

    std::optional<size_t> lookup(const std::string& name, const std::vector<Namespace<T>>& all_namespaces) const {
        auto it = symbol_table.find(name);
        if (it != symbol_table.end()) return it->second;
        if (parent_namespace_idx) return all_namespaces[*parent_namespace_idx].lookup(name, all_namespaces);
        return std::nullopt;
    }
};

#endif // NAMESPACE_H
