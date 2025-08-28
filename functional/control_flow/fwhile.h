#ifndef FWHILE_H
#define FWHILE_H

#include "../iterators/break_fit.h"
#include <functional>
#include <variant>

// Functional while loop returning bool
template <typename ConditionFn, typename ActionFn>
bool fwhile(ConditionFn condition, ActionFn actionFn) {
    BreakIterator iterator;
    bool finalResult = false;

    // Wrap iteration as TailCall
    std::function<TailCall<bool>()> loop = [&]() -> TailCall<bool> {
        return TailCall<bool>([&]() -> bool {
            // Evaluate condition
            if constexpr (std::is_invocable_v<ConditionFn>) {
                if (!condition()) {
                    iterator.setEnd(true);
                    return finalResult;
                }
            } else {
                if (!condition) {
                    iterator.setEnd(true);
                    return finalResult;
                }
            }

            // Run action
            finalResult = actionFn();

            // Recursive call
            auto next = iterator.iterate(actionFn);
            if (std::holds_alternative<std::function<TailCall<bool>()>>(next)) {
                return std::get<std::function<TailCall<bool>()>>(next)().func();
            } else {
                return finalResult;
            }
        });
    };

    return loop().func(); // execute loop
}

#endif // FWHILE_H
