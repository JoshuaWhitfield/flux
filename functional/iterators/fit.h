#ifndef FITERATOR_H
#define FITERATOR_H

#include <functional>
#include <variant>
#include <vector>
#include "../models/edge.h"
#include "../models/node.h"
#include "../models/tco.h"

// TailCall wrapper for TCO
template <typename T>
inline auto wrap_history(const std::vector<T>& h) {
    return std::variant<std::function<TailCall<std::vector<T>>()>, std::vector<T>>(h);
}

template <typename T, typename R, typename NodeT>
class Iterator {
private:
    NodeT& node;

public:
    Edge<int>& index;
    Edge<std::optional<T>>& elem;
    Edge<int>& interval;
    Edge<std::vector<T>>& body;
    Edge<std::vector<R>>& history;
    Edge<bool>& back;
    Edge<bool>& end;
    Edge<bool>& cont;

    Iterator(
        NodeT& _node,
        Edge<int>& _index,
        Edge<std::optional<T>>& _elem,
        Edge<int>& _interval,
        Edge<std::vector<T>>& _body,
        Edge<std::vector<R>>& _history,
        Edge<bool>& _back,
        Edge<bool>& _end,
        Edge<bool>& _cont
    ) : node(_node), index(_index), elem(_elem), interval(_interval),
        body(_body), history(_history), back(_back), end(_end), cont(_cont) {
        
        // Initialize the node with all the edges
        node["index"] = index;
        node["elem"] = elem;
        node["interval"] = interval;
        node["body"] = body;
        node["history"] = history;
        node["back"] = back;
        node["end"] = end;
        node["cont"] = cont;
    }

    // Converts actionFn(Node) -> R
    auto iterationAction(std::function<R(NodeT&)> actionFn, NodeT& node) {
        return actionFn(node);
    }

    auto iterate() {
        return TailCall<bool>([this]() -> std::variant<std::function<TailCall<bool>()>, bool> {
            int next = index.get() + interval.get();
            if (next >= static_cast<int>(body.get().size())) {
                end.set(true);
                return false;
            }
            index.set(next);
            elem.set(body.get()[index.get()]); // Fixed: was 'iterand', should be 'elem'
            
            // Update the node with new values
            node["index"] = index;
            node["elem"] = elem;
            
            return true;
        });
    }

    auto backtrack() {
        return TailCall<bool>([this]() -> std::variant<std::function<TailCall<bool>()>, bool> {
            int prev = index.get() - interval.get();
            if (prev < 0) {
                back.set(false);
                return false;
            }
            index.set(prev);
            elem.set(body.get()[index.get()]); // Fixed: was 'iterand', should be 'elem'
            back.set(false);
            
            // Update the node with new values
            node["index"] = index;
            node["elem"] = elem;
            node["back"] = back;
            
            return true;
        });
    }

    // Declare member templates with explicit return types
    template <typename ActionFn>
    TailCall<std::vector<R>> iterating(ActionFn actionFn, NodeT& node);

    template <typename ActionFn>
    TailCall<std::vector<R>> backtracking(ActionFn actionFn, NodeT& node);
};

// -----------------------
// Template Definitions Outside Class
// -----------------------

template <typename T, typename R, typename NodeT>
template <typename ActionFn>
TailCall<std::vector<R>> Iterator<T, R, NodeT>::iterating(ActionFn actionFn, NodeT& node) {
    return TailCall<std::vector<R>>([this, &actionFn, &node]() -> std::variant<std::function<TailCall<std::vector<R>>()>, std::vector<R>> {
        if (back.get()) {
            return wrap_history(this->backtracking(actionFn, node).run());
        }

        if (end.get()) return wrap_history(history.get());

        auto valid = iterate().run();
        if (!valid) return wrap_history(history.get());

        auto h = history.get();
        if (elem.get().has_value()) { // Fixed: was 'iterand', should be 'elem', and check if optional has value
            h.push_back(iterationAction(actionFn, node));
        }
        history.set(h);
        
        // Update node with new history
        this->node["history"] = history;

        return [this, &actionFn, &node]() { return this->iterating(actionFn, node); };
    });
}

template <typename T, typename R, typename NodeT>
template <typename ActionFn>
TailCall<std::vector<R>> Iterator<T, R, NodeT>::backtracking(ActionFn actionFn, NodeT& node) {
    return TailCall<std::vector<R>>([this, &actionFn, &node]() -> std::variant<std::function<TailCall<std::vector<R>>()>, std::vector<R>> {
        auto valid = backtrack().run();
        if (!valid) return wrap_history(history.get());

        auto h = history.get();
        if (!h.empty() && elem.get().has_value()) { // Fixed: was 'iterand', should be 'elem', and check if optional has value
            h.back() = iterationAction(actionFn, node);
        }
        history.set(h);
        
        // Update node with new history
        this->node["history"] = history;

        return [this, &actionFn, &node]() { return this->iterating(actionFn, node); };
    });
}

#endif // FITERATOR_H