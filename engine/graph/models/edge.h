#ifndef EDGE_H
#define EDGE_H

struct Edge {
    uint32_t source;
    uint32_t target;
    uint32_t label;

    inline Edge(const uint32_t& _source, const uint32_t& _target, const uint32_t& _label)
        : source(_source), target(_target), label(_label) {}
};

inline std::vector<Edge> edges;

#endif // EDGE_H