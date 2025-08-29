package functional.controlFlow;

import functional.iterators.BreakFIterator;
import functional.models.TailCall;

import java.util.function.Supplier;

public class FWhile {

    public static boolean fwhile(
        Supplier<Boolean> condition, 
        Supplier<Boolean> actionFn
    ) {
        BreakFIterator iterator = new BreakFIterator();
        final boolean[] finalResult = {false};

        TailCall<Boolean> loop = new TailCall<>(() -> {
            if (!condition.get()) {
                iterator.getState().setEnd(true);
                return finalResult[0];
            }

            finalResult[0] = actionFn.get();

            // Recursive TailCall
            return iterator.iterate(actionFn).tco_run();
        });

        return loop.tco_run();
    }

}