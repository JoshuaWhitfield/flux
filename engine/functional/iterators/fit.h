#ifndef FUNCTIONALITERATOR_H
#define FUNCTIONALITERATOR_H

#include <functional>  // for std::function
#include <variant>     // for std::variant (tail-call wrapper)
#include <vector>      // for history or body containers
#include "../models/edge.h"
#include "../models/tco.h"

// TailCall wrapper for TCO

template <typename T, typename NodeT>
class Iterator {
public:
    Edge<int> index;
    Edge<T> iterand;
    Edge<int> interval;
    Edge<std::vector<T>> body;
    Edge<std::vector<T>> history;
    Edge<bool> back;
    Edge<bool> end;
    Edge<bool> cont;

    // Constructor
    Iterator(int startIndex, T firstIterand, int step, std::vector<T> iterableBody)
        : index(startIndex),
          iterand(firstIterand),
          interval(step),
          body(iterableBody),
          history({}),
          back(false),
          end(false),
          cont(false)
    {}

    // Executes a single iteration action
    auto iterationAction(std::function<T(NodeT&)> actionFn, NodeT& node) {
        return actionFn(node);
    }

    // Moves iterator forward
    auto iterate() {
        return TailCall<bool>([this]() -> std::variant<std::function<TailCall<bool>()>, bool> {
            index.set(index.get() + interval.get());
            iterand.set(body.get()[index.get()]);
            return iterand.get() != T{};
        });
    }

    // Moves iterator backward (backtracking)
    auto backtrack() {
        return TailCall<bool>([this]() -> std::variant<std::function<TailCall<bool>()>, bool> {
            index.set(index.get() - interval.get());
            iterand.set(body.get()[index.get()]);
            back.set(false);
            return iterand.get() != T{};
        });
    }

    // Main iteration logic (tail-call style)
    template <typename ActionFn>
    auto iterating(ActionFn actionFn, NodeT& node) {
        return TailCall<std::vector<T>>([this, &actionFn, &node]() -> std::variant<std::function<TailCall<std::vector<T>>()>, std::vector<T>> {
            if (back.get()) {
                return [this, &actionFn, &node]() { return this->backtracking(actionFn, node); };
            }
            if (cont.get()) {
                cont.set(false);
                auto h = history.get();
                if (!h.empty()) h.pop_back();
                history.set(h);
                return [this, &actionFn, &node]() { return this->iterating(actionFn, node); };
            }
            if (end.get()) return history.get();

            auto valid = iterate().run();   // run the TailCall<bool> to get boolean
            if (valid) {
                auto h = history.get();
                h.push_back(iterationAction(actionFn, node));
                history.set(h);
                return [this, &actionFn, &node]() { return this->iterating(actionFn, node); };
            } else {
                return history.get();
            }
        });
    }


    // Backtracking logic
    template <typename ActionFn>
    auto backtracking(ActionFn actionFn, NodeT& node) {
        return TailCall<std::vector<T>>([this, &actionFn, &node]() -> std::variant<std::function<TailCall<std::vector<T>>()>, std::vector<T>> {
            auto valid = backtrack().run(); // run TailCall<bool>
            if (valid) {
                auto h = history.get();
                if (!h.empty()) h.back() = iterationAction(actionFn, node);
                history.set(h);
                return [this, &actionFn, &node]() { return this->iterating(actionFn, node); };
            } else {
                return history.get();
            }
        });
    }

};

#endif // FUNCTIONALITERATOR_H
