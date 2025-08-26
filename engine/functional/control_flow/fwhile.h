#ifndef FWHILE_HPP
#define FWHILE_HPP

#include <functional>
#include "break_iterator.hpp"
#include "node.hpp"

template <typename Container>
void FWHILE(
    std::function<bool(Node&)> condition,     // condition predicate
    std::function<void(Node&)> actionFn,      // action function
    Container& iterable,
    int startAt = 0,
    int jump = 1
) {
    Node node(iterable);

    BreakIterator iterator(startAt, jump, iterable);

    iterator.iterate(node, [&](Node& n) {
        // evaluate user-provided condition
        if (!condition(n)) {
            iterator.getState().setEnd(true);   // break loop
            return;
        }

        // run user-provided action
        actionFn(n);
    });
}

#endif // FWHILE_HPP
