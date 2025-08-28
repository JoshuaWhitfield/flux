package functional.control_flow;
import java.util.function.*;

import static functional.control_flow.Fif.*;
import functional.control_flow.Callables.VoidCallable;
import functional.control_flow.Callables.BoolCallable;
import functional.control_flow.Callables.AnyCallable;


public class Bincase {
    /**
     * Invokes a value if it's callable, otherwise return the value itself
     */

    public static <T> T invokeIfCallable(Object value) {
        if (value instanceof Supplier) {
            @SuppressWarnings("unchecked")
            Supplier<T> supplier = (Supplier<T>) value;
            return supplier.get();
        } else if (value instanceof VoidCallable) {
            ((VoidCallable) value).call();
            return null;
        } else if (value instanceof BoolCallable) {
            boolean result = ((BoolCallable) value).call();
            @SuppressWarnings("unchecked")
            T encapsulated = (T) (Boolean) result;
            return encapsulated;
        } else if (value instanceof Object) {
            @SuppressWarnings("unchecked")
            T result = ((AnyCallable<T>) value).call();
            return result;
        } else {
            @SuppressWarnings("unchecked")
            T result = (T) value;
            return result;
        }
    }

    /**
     * Evaluates a conditional that can be a bolean literal or a boolean-returning callable
     */

    public static boolean evaluateCondition(Object conditional) {
        if (conditional instanceof Boolean) {
            return (Boolean) conditional; 
        } else if (conditional instanceof BooleanSupplier) {
            return ((BooleanSupplier) conditional).getAsBoolean();
        } else if (conditional instanceof Supplier) {
            Object result = ((Supplier<?>) conditional).get();
            if (result instanceof Boolean) {
                return (Boolean) result;
            }
            throw new IllegalArgumentException("Supplier must return Boolean");
        } else {
            throw new IllegalArgumentException("Conditional must be boolean or Boolean Supplier");
        }
    }

    // =====================================================
    // BINCASE IMPLEMENTATIONS WITH METHOD OVERLOADING
    // =====================================================

    /**
     * Main bincase method - most flexible version.
     * Handles an combination of literals and callables.
     */

    public static <T> T bincase(Object conditional, Object onSuccess, Object onFailure) {
        // Evaluate the conditional
        boolean condResult = evaluateCondition(conditional);

        // Use fif to handle the branching
        return fif(
            () -> condResult,
            () -> invokeIfCallable(onSuccess),
            () -> invokeIfCallable(onFailure)
        );
    }

    /**
     * Type-safe version with explicit Supplier types.
    */

    public static <T> T bincase(
        BooleanSupplier conditional,
        Supplier<T> onSuccess,
        Supplier<T> onFailure    
    ) {
        boolean condResult = conditional.getAsBoolean();
        return fif(() -> condResult, onSuccess, onFailure);
    }

    /**
     * Version with literal boolean condition.
     */

    public static <T> T bincase(
        boolean conditional,
        Supplier<T> onSuccess,
        Supplier<T> onFailure
    ) {
        return fif(() -> conditional, onSuccess, onFailure);
    }

    /**
     * Version with mixed literal/callable parameters
     */

    public static <T> T bincase(
        boolean conditional,
        T onSuccess,
        T onFailure
    ) {
        return fif(() -> conditional, () -> onSuccess, () -> onFailure);
    }

    /**
     * Version supporting void operations.
     */

    public static void bincaseVoid(
        Object conditional,
        VoidCallable onSuccess,
        VoidCallable onFailure
    ) {
        boolean condResult = evaluateCondition(conditional);
        fif_void(() -> condResult, onSuccess, onFailure);
    }
}