package functional.controlFlow;

import functional.controlFlow.Callables.*;
import java.util.function.BooleanSupplier;
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
        return condition.getAsBoolean() ? onTrue.get() : onFalse.get();
    }

    /**
     * Functional if implementation - evaluates condition and returns appropriate branch. 
     */
    public static <T> T fif(
        boolean condition,
        Supplier<T> onTrue,
        Supplier<T> onFalse
    ) {
        return condition ? onTrue.get() : onFalse.get();
    }

    /**
     * Void version of functinoal if.
     */

    public static void fif_void(
        BooleanSupplier condition,
        VoidCallable onTrue,
        VoidCallable onFalse
    ) {
        if (condition.getAsBoolean()) {
            onTrue.call();
        } else {
            onFalse.call();
        }
    }
}