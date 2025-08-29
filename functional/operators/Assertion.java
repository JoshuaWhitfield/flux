package functional.operators;

import java.util.function.*;
import java.lang.reflect.*;

public final class Assertion {

    private Assertion() {} // prevent instantiation

    // ----------------------------
    // ASSERT
    // ----------------------------
    // Executes conditionFn, returns the value if true, otherwise throws RuntimeException
    public static <T> T fassert(Supplier<Boolean> conditionFn, T returnValue) {
        if (conditionFn.get()) {
            return returnValue;
        } else {
            throw new RuntimeException("Assertion failed");
        }
    }

    // ----------------------------
    // CALLABLE
    // ----------------------------
    // Checks if a function can be invoked with given args
    // (Reflection-based approximation, since Java lacks is_invocable)
    public static boolean fcallable(Object func, Object... args) {
        if (func instanceof Method) {
            Method m = (Method) func;
            Class<?>[] params = m.getParameterTypes();
            if (params.length != args.length) return false;
            for (int i = 0; i < params.length; i++) {
                if (args[i] != null && !params[i].isAssignableFrom(args[i].getClass())) {
                    return false;
                }
            }
            return true;
        }
        if (func instanceof java.util.function.Function) {
            // Can't strongly verify in Java generics due to type erasure → assume callable
            return args.length == 1;
        }
        if (func instanceof java.util.function.Supplier) {
            return args.length == 0;
        }
        if (func instanceof java.util.function.Consumer) {
            return args.length == 1;
        }
        return false;
    }

    // ----------------------------
    // TYPE
    // ----------------------------
    // Returns type name of value as string
    public static <T> String ftype(T value) {
        if (value == null) return "null";
        return value.getClass().getTypeName();
    }
}
