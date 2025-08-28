#ifndef BREAK_FITERATOR_H
#define BREAK_FITERATOR_H

#include <functional>
#include <variant>
#include <iostream>

#include "../models/tco.h"

// Holds loop control state
struct IteratorState {
    bool end = false;
    bool cont = false;
    bool back = false;

    bool getEnd() const { return end; }
    void setEnd(bool val) { end = val; }

    bool getCont() const { return cont; }
    void setCont(bool val) { cont = val; }

    bool getBack() const { return back; }
    void setBack(bool val) { back = val; }
};

// BreakIterator specialized for bool-returning loops
class BreakIterator {
public:
    IteratorState state;

    template <typename ActionFn>
    std::variant<std::function<TailCall<bool>()>, std::monostate>
    iterate(ActionFn actionFn) {
        // Use bincase / fif style branching
        return [&]() -> std::variant<std::function<TailCall<bool>()>, std::monostate> {
            if (!actionFn()) { // use actionFn as condition here
                state.setEnd(true);
                return std::monostate{};
            }

            // Continue iteration
            return std::function<TailCall<bool>()>([&]() {
                return iterate(actionFn);
            });
        }();
    }

    bool getEnd() const { return state.getEnd(); }
    void setEnd(bool val) { state.setEnd(val); }
    bool getCont() const { return state.getCont(); }
    void setCont(bool val) { state.setCont(val); }
};

#endif // BREAK_FITERATOR_H
