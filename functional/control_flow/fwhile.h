#ifndef FWHILE_H
#define FWHILE_H

#include <functional>
#include "../iterators/break_fit.h"
#include "../models/node.h"

template <typename T, typename ConditionFn, typename ActionFn>
inline void fwhile(
    ConditionFn condition,     // condition predicate
    ActionFn actionFn      // action function
) {
    BreakIterator<T, T> iterator;

    iterator.iterate([&]() {
        // evaluate user-provided condition
        if (!condition()) {
            iterator.setEnd(true);   // break loop
            return;
        }

        // run user-provided action
        actionFn(); // return value ignored
    });
}

#endif // FWHILE_H
