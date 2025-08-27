#ifndef FWHILE_H
#define FWHILE_H

#include <functional>
#include "../iterators/break_fit.h"
#include "../models/node.h"

template <typename Container>
inline void fwhile(
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

#endif // FWHILE_H
