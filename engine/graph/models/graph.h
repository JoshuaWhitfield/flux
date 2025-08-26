#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <functional>

#include "./node.h"
#include "./edge.h"
#include "../constants/edge_const.h"

struct Graph {
    std::vector<Node<T>>& nodes;
    const std::vector<Edge<T>> edges;
    
    Graph(std::vector<Node<T>>& _nodes, const std::vector<Edge>& _edges)
        : nodes(_nodes), edges(_edges) {}
    
    // example traversal: get connected nodes for a given node id
    std::vector<std::reference_wrapper<const Node<T>>> get_conn_nodes(size_t node_id) const {
        std::vector<std::reference_wrapper<const Node<T>>> result;

        if (node_id >= nodes.size()) return result;

        const Node<T>& node = nodes[node_id];
        for (size_t edge_idx : node.edge_indices()) {
            if (edge_idx < edges.size()) {
                size_t target_id = edges[edge_idx].target;
                if (target_id < nodes.size()) result.push_back(std::cref(nodes[target_id]));
            }
        }
        return result;
    }

    // optional: get edges from a node
    std::vector<std::reference_wrapper<const Edge>> get_edges(size_t node_id) const {
        std::vector<std::reference_wrapper<const Edge>> result;
        if (node_id >= nodes.size()) return result;

        const Node<T>& node = nodes[node_id];
        for (size_t edge_idx : node.edge_indices()) {
            if (edge_idx < edges.size()) result.push_back(std::cref(edges[edge_idx]));
        }
        return result;
    }
}

#endif // GRAPH_H
