#ifndef FITERATOR_H
#define FITERATOR_H

#include <functional>
#include <variant>
#include <vector>
#include "../models/edge.h"
#include "../models/tco.h"

// TailCall wrapper for TCO
template <typename T>
inline auto wrap_history(const std::vector<T>& h) {
    return std::variant<std::function<TailCall<std::vector<T>>()>, std::vector<T>>(h);
}

template <typename T, typename R, typename NodeT>
class Iterator {
public:
    Edge<int>& index;
    Edge<std::optional<T>>& iterand;
    Edge<int>& interval;
    Edge<std::vector<T>>& body;
    Edge<std::vector<R>>& history;
    Edge<bool>& back;
    Edge<bool>& end;
    Edge<bool>& cont;

    Iterator(
        Edge<int>& idx,
        Edge<std::optional<T>>& iter,
        Edge<int>& step,
        Edge<std::vector<T>>& b,
        Edge<std::vector<R>>& h,
        Edge<bool>& bk,
        Edge<bool>& en,
        Edge<bool>& ct
    ) : index(idx), iterand(iter), interval(step),
        body(b), history(h), back(bk), end(en), cont(ct) {}

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
            iterand.set(body.get()[index.get()]);
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
            iterand.set(body.get()[index.get()]);
            back.set(false);
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
    return TailCall<std::vector<R>>([this, &actionFn, &node]() -> std::variant<std::function<TailCall<std::vector<R>>()> , std::vector<R>> {
        if (back.get()) {
            return wrap_history(this->backtracking(actionFn, node).run());
        }

        if (end.get()) return wrap_history(history.get());

        auto valid = iterate().run();
        if (!valid) return wrap_history(history.get());

        auto h = history.get();
        if (iterand.get()) h.push_back(iterationAction(actionFn, node));
        history.set(h);

        return [this, &actionFn, &node]() { return this->iterating(actionFn, node); };
    });
}

template <typename T, typename R, typename NodeT>
template <typename ActionFn>
TailCall<std::vector<R>> Iterator<T, R, NodeT>::backtracking(ActionFn actionFn, NodeT& node) {
    return TailCall<std::vector<R>>([this, &actionFn, &node]() -> std::variant<std::function<TailCall<std::vector<R>>()> , std::vector<R>> {
        auto valid = backtrack().run();
        if (!valid) return wrap_history(history.get());

        auto h = history.get();
        if (!h.empty() && iterand.get()) h.back() = iterationAction(actionFn, node);
        history.set(h);

        return [this, &actionFn, &node]() { return this->iterating(actionFn, node); };
    });
}

#endif // FITERATOR_H
