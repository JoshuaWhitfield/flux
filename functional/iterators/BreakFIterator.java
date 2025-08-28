package functional.iterators;

import functional.iterators.states.IteratorState;
import functional.models.TailCall;
import java.util.function.Supplier;

public class BreakFIterator {
    private final IteratorState state = new IteratorState();
    
    public IteratorState getState() { return state; }

    public TailCall<Boolean> iterate(Supplier<Boolean> actionFn) {
        return new TailCall<>(() -> {
            if (!actionFn.get()) {
                state.setEnd(true);
                return false;
            }

            // Recursive TailCall
            return iterate(actionFn).tco_run();
        });
    }
}
