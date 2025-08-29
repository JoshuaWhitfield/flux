package functional.operators;

import java.util.*;
import java.lang.reflect.*;

public final class Comparisons {

    private Comparisons() {} // prevent instantiation

    // feq: functional equality
    // returns left if equal, Optional.empty() otherwise
    public static <T> Optional<T> feq(T left, T right) {
        if (Objects.equals(left, right)) {
            return Optional.of(left);
        }
        return Optional.empty();
    }

    // fneq: functional not-equal
    public static <T> boolean fneq(T left, T right) {
        return !Objects.equals(left, right);
    }

    // fgeq: functional greater-or-equal
    // returns whichever is "greater" by size() if available, else Comparable
    @SuppressWarnings("unchecked")
    public static <T> T fgeq(T left, T right) {
        try {
            Method sizeMethodLeft = left.getClass().getMethod("size");
            Method sizeMethodRight = right.getClass().getMethod("size");
            int sizeLeft = (int) sizeMethodLeft.invoke(left);
            int sizeRight = (int) sizeMethodRight.invoke(right);
            if (sizeLeft > sizeRight) return left;
            if (sizeRight > sizeLeft) return right;
            return left; // equal sizes
        } catch (Exception e) {
            // fallback: use Comparable
            if (left instanceof Comparable && right instanceof Comparable) {
                Comparable<Object> lc = (Comparable<Object>) left;
                return (lc.compareTo(right) >= 0) ? left : right;
            }
            throw new IllegalArgumentException("Types not comparable: " + left + " vs " + right);
        }
    }

    // fleq: functional less-or-equal
    // returns left if <= right, Optional.empty otherwise
    @SuppressWarnings("unchecked")
    public static <T> Optional<T> fleq(T left, T right) {
        if (left instanceof Comparable && right instanceof Comparable) {
            Comparable<Object> lc = (Comparable<Object>) left;
            if (lc.compareTo(right) <= 0) {
                return Optional.of(left);
            }
        }
        return Optional.empty();
    }

    // fgt: functional greater-than
    @SuppressWarnings("unchecked")
    public static <T> boolean fgt(T left, T right) {
        if (left instanceof Comparable && right instanceof Comparable) {
            Comparable<Object> lc = (Comparable<Object>) left;
            return lc.compareTo(right) > 0;
        }
        throw new IllegalArgumentException("Types not comparable");
    }

    // flt: functional less-than
    @SuppressWarnings("unchecked")
    public static <T> boolean flt(T left, T right) {
        if (left instanceof Comparable && right instanceof Comparable) {
            Comparable<Object> lc = (Comparable<Object>) left;
            return lc.compareTo(right) < 0;
        }
        throw new IllegalArgumentException("Types not comparable");
    }

    // fpeq: functional plus-equal
    public static double fpeq(Number left, Number right) {
        return left.doubleValue() + right.doubleValue();
    }

    // fseq: functional subtract-equal
    public static double fseq(Number left, Number right) {
        return left.doubleValue() - right.doubleValue();
    }

    // fmeq: functional multiply-equal
    public static double fmeq(Number left, Number right) {
        return left.doubleValue() * right.doubleValue();
    }

    // fdeq: functional divide-equal
    public static Optional<Double> fdeq(Number left, Number right) {
        if (right.doubleValue() == 0.0) return Optional.empty();
        return Optional.of(left.doubleValue() / right.doubleValue());
    }

    // fmdeq: functional modulo-equal
    public static Optional<Long> fmdeq(Number left, Number right) {
        if (right.longValue() == 0) return Optional.empty();
        return Optional.of(left.longValue() % right.longValue());
    }

    // fxeq: functional xor-equal
    public static Optional<Long> fxeq(Number left, Number right) {
        return Optional.of(left.longValue() ^ right.longValue());
    }
}
