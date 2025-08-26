#ifndef FFOR_H
#define FFOR_H

#include "../iterators/fit.h"
#include "../models/edge.h"
#include "../models/node.h"
#include "bincase.h"
#include "fif.h"
#include <vector>
#include <functional>

// Functional FOR wrapper
template <typename T>
auto ffor(
    std::vector<T> iterable,
    std::function<T(Node&)> actionFn,
    int startAt = -1,
    int jump = 1
) {
    // Create node with Edges
    Node node;
    node["index"]   = Edge<int>(startAt);
    node["elem"]    = Edge<T>(T{});
    node["jump"]    = Edge<int>(jump);
    node["self"]    = Edge<std::vector<T>>(iterable);
    node["history"] = Edge<std::vector<T>>({});
    node["back"]    = Edge<bool>(false);
    node["end"]     = Edge<bool>(false);
    node["cont"]    = Edge<bool>(false);

    // Shortcut references
    auto& index   = node["index"];
    auto& elem    = node["elem"];
    auto& jumpEdge= node["jump"];
    auto& self    = node["self"];
    auto& history = node["history"];
    auto& back    = node["back"];
    auto& end     = node["end"];
    auto& cont    = node["cont"];

    // Create iterator
    Iterator<T, Node> iterator(
        index,
        elem,
        jumpEdge,
        self,
        history,
        back,
        end,
        cont
    );

    // FDEC-style declarative wrapper
    return bincase(
        fif(end.get(),
            []() { return true; },
            []() { return false; }
        ),
        // onFailure: backtracking and end
        [&]() {
            iterator.backtracking(actionFn, node); // backtrack
            return node["history"].get();
        },
        // onSuccess: iterate normally
        [&]() {
            return iterator.iterating(actionFn, node).run();
        }
    );
}

#endif // FFOR_H