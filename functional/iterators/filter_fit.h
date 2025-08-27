#ifndef FILTER_ITERATOR_H
#define FILTER_ITERATOR_H

#include <functional>
#include <variant>
#include <vector>
#include <optional>
#include "../control_flow/normalize.h"
#include "../models/edge.h"
#include "../models/node.h"
#include "../models/tco.h"

template <typename T, typename NodeT>
class FilterIterator {
public:
    Edge<int> index;
    Edge<T> iterand;
    Edge<int> interval;
    Edge<std::vector<T>> body;
    Edge<std::vector<T>> history;
    Edge<bool> end;

    // Constructor
    FilterIterator(int startIndex, T firstIterand, int step, std::vector<T> iterableBody)
        : index(startIndex),
          iterand(firstIterand),
          interval(step),
          body(iterableBody),
          history({}),
          end(false)
    {}

    // Executes callback and normalizes the result
    template <typename ActionFn>
    auto iterationAction(ActionFn actionFn, NodeT& node) {
        auto raw = actionFn(node, iterand.get());
        return normalize(raw);  // → std::optional<U>
    }

    // Move iterator forward
    auto iterate() {
        return TailCall<bool>([this]() -> std::variant<std::function<TailCall<bool>()>, bool> {
            index.set(index.get() + interval.get());
            if (index.get() >= static_cast<int>(body.get().size())) {
                end.set(true);
                return false;
            }
            iterand.set(body.get()[index.get()]);
            return true;
        });
    }

    template <typename ActionFn>
    TailCall<std::vector<T>> filtering(ActionFn actionFn, NodeT& node);

    // Main filtering logic
};

template <typename T, typename NodeT>
template <typename ActionFn>
TailCall<std::vector<T>> FilterIterator<T, NodeT>::filtering(ActionFn actionFn, NodeT& node) {
    return TailCall<std::vector<T>>([this, &actionFn, &node]() -> std::variant<std::function<TailCall<std::vector<T>>()>, std::vector<T>> {
        if (end.get()) return history.get();

        auto valid = iterate().run();   // step forward
        if (!valid) return history.get();

        auto normalized = iterationAction(actionFn, node);
        if (normalized) {
            auto h = history.get();
            h.push_back(*normalized);
            history.set(h);
        }

        // Recursive step properly wrapped as variant
        return std::function<TailCall<std::vector<T>>()>([this, &actionFn, &node]() {
            return this->filtering(actionFn, node);
        });
    });
}



#endif // FILTER_ITERATOR_H
