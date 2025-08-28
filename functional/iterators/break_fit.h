#ifndef BREAK_FITERATOR_H
#define BREAK_FITERATOR_H

#include <functional>
#include <variant>
#include <utility>      // for std::move
#include <vector>
#include "../models/tco.h"
#include "../control_flow/fif.h"
#include "../control_flow/callable.h"
#include <optional>
#include <string>
#include <iostream>
#include <cassert>
#include <variant>      // std::monostate

// Holds state of break/continue/end for the iterator
struct IteratorState {
    bool end = false;     // break condition
    bool cont = false;    // continuation
    bool back = false;    // backtracking

    // Getters
    bool getEnd() const { return end; }
    bool getCont() const { return cont; }
    bool getBack() const { return back; }

    // Setters
    void setEnd(bool val) { end = val; }
    void setCont(bool val) { cont = val; }
    void setBack(bool val) { back = val; }
};

// Functional iterator that uses IteratorState for control
template <typename T, typename NodeT>
class BreakIterator {
public:
    IteratorState state;

    // Single iteration step
    template <typename ActionFn>
    auto iterate(ActionFn actionFn) {
        return TailCall<std::monostate>([this, actionFn]() -> std::variant<std::function<TailCall<std::monostate>()>, std::monostate> {
            // Execute action (if callable), ignore return value
            if (callable(actionFn)) { actionFn(); }
            // Tail-call next iteration
            return [this, actionFn]() { return iterate(actionFn); };
        });
    }
    // ...existing code...

    // Expose state getters/setters for frontend usage
    bool getEnd() const { return state.getEnd(); }
    void setEnd(bool val) { state.setEnd(val); }
    bool getCont() const { return state.getCont(); }
    void setCont(bool val) { state.setCont(val); }
    bool getBack() const { return state.getBack(); }
    void setBack(bool val) { state.setBack(val); }
};

#endif // BREAK_FITERATOR_H
