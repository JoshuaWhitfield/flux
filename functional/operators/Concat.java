package functional.operators;

import java.util.*;

public final class Concat {

    private Concat() {} // prevent instantiation

    // ----------------------------
    // Generic fcat
    // ----------------------------
    // Fallback: try to use + (works naturally for numbers and strings in Java)
    public static <T, U> Object fcat(T a, U b) {
        // String concatenation case
        if (a instanceof String || b instanceof String) {
            return String.valueOf(a) + String.valueOf(b);
        }

        // Numeric addition (if both are numbers)
        if (a instanceof Number && b instanceof Number) {
            // Promote to double for simplicity
            return ((Number) a).doubleValue() + ((Number) b).doubleValue();
        }

        // Unsupported types (Java has no operator overloading)
        throw new UnsupportedOperationException(
            "fcat not supported for types: " + a.getClass() + " and " + b.getClass()
        );
    }

    // ----------------------------
    // Vector/List specialization
    // ----------------------------
    public static <T> List<T> fcat(List<T> a, List<T> b) {
        List<T> result = new ArrayList<>(a);
        result.addAll(b);
        return result;
    }

    // ----------------------------
    // Tuple equivalent (Java has no std::tuple)
    // ----------------------------
    // → We can approximate with Map.Entry or a custom Pair
    public static <A, B, C, D> List<Object> fcat(Pair<A, B> a, Pair<C, D> b) {
        return Arrays.asList(a.first, a.second, b.first, b.second);
    }

    // ----------------------------
    // String + String (explicit overload for clarity)
    // ----------------------------
    public static String fcat(String a, String b) {
        return a + b;
    }

    // ----------------------------
    // String + Anything
    // ----------------------------
    public static <T> String fcat(String a, T b) {
        return a + String.valueOf(b);
    }

    public static <T> String fcat(T a, String b) {
        return String.valueOf(a) + b;
    }

    // Simple Pair class since Java has no std::tuple
    public static class Pair<X, Y> {
        public final X first;
        public final Y second;
        public Pair(X first, Y second) {
            this.first = first;
            this.second = second;
        }
    }
}
