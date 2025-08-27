#ifndef FFOR_H
#define FFOR_H

#include "../iterators/fit.h"
#include "../models/edge.h"
#include "../models/node.h"
#include "./bincase.h"
#include "./fif.h"
#include <vector>
#include <functional>

// Functional FOR wrapper
template <typename T, typename R>
inline auto ffor(
    std::vector<T> iterable,
    std::function<R(Node&)> actionFn,  // now uses heterogeneous Node
    int startAt = -1,
    int jump = 1
) {
    Node node;  // heterogeneous node

    node["index"]   = Edge<int>(startAt);
    node["elem"]    = Edge<std::optional<T>>(std::nullopt);
    node["jump"]    = Edge<int>(jump);
    node["self"]    = Edge<std::vector<T>>(iterable);
    node["history"] = Edge<std::vector<R>>({});
    node["back"]    = Edge<bool>(false);
    node["end"]     = Edge<bool>(false);
    node["cont"]    = Edge<bool>(false);

    // Typed references into the heterogeneous node
    auto& index    = node.getAs<Edge<int>>("index");
    auto& elem     = node.getAs<Edge<std::optional<T>>>("elem");
    auto& jumpEdge = node.getAs<Edge<int>>("jump");
    auto& self     = node.getAs<Edge<std::vector<T>>>("self");
    auto& history  = node.getAs<Edge<std::vector<R>>>("history");
    auto& back     = node.getAs<Edge<bool>>("back");
    auto& end      = node.getAs<Edge<bool>>("end");
    auto& cont     = node.getAs<Edge<bool>>("cont");

    Iterator<T, R, Node> iterator(  // heterogeneous Node type
        index,
        elem,
        jumpEdge,
        self,
        history,
        back,
        end,
        cont
    );

    return bincase(
        fif(
            end.get(),
            []() { return true; },
            []() { return false; }
        ),
        [&]() {
            return iterator.iterating(actionFn, node).run();
        },
        [&]() { return history.get(); } 
    );


}

#endif // FFOR_H
