package functional.control_flow;

import functional.control_flow.Callables.*;
import java.util.function.Supplier;

public class Fif {
    /**
     * Functional if implementation - evaluates condition and returns appropriate branch. 
     */
    public static <T> T fif(
        BooleanSupplier condition,
        Supplier<T> onTrue,
        Supplier<T> onFalse
    ) {
        return condition.get() ? onTrue.get() : onFalse.get();
    }

    /**
     * Void version of functinoal if.
     */

    public static void fif_void(
        BooleanSupplier condition,
        VoidCallable onTrue,
        VoidCallable onFalse
    ) {
        if (condition.get()) {
            onTrue.call();
        } else {
            onFalse.call();
        }
    }
}