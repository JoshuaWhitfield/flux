package functional.operators;

import java.util.Optional;

public final class Mathematic {

    private Mathematic() {} // prevent instantiation

    // fmin: subtraction (-)
    public static <T extends Number, U extends Number> double fmin(T left, U right) {
        return left.doubleValue() - right.doubleValue();
    }

    // fadd: addition (+)
    public static <T extends Number, U extends Number> double fadd(T left, U right) {
        return left.doubleValue() + right.doubleValue();
    }

    // fmul: multiplication (*)
    public static <T extends Number, U extends Number> double fmul(T left, U right) {
        return left.doubleValue() * right.doubleValue();
    }

    // fdiv: division (/)
    // returns Optional.empty() on divide-by-zero
    public static <T extends Number, U extends Number> Optional<Double> fdiv(T left, U right) {
        if (right.doubleValue() == 0.0) return Optional.empty();
        return Optional.of(left.doubleValue() / right.doubleValue());
    }

    // fmod: modulo (%)
    // only meaningful for integral types
    public static <T extends Number, U extends Number> Optional<Long> fmod(T left, U right) {
        if (right.longValue() == 0) return Optional.empty();
        return Optional.of(left.longValue() % right.longValue());
    }

    // fexp: exponentiation (^ -> math power, not bitwise)
    public static <T extends Number, U extends Number> double fexp(T base, U exponent) {
        return Math.pow(base.doubleValue(), exponent.doubleValue());
    }

    // finc: increment (++)
    public static <T extends Number> double finc(T value) {
        return value.doubleValue() + 1;
    }

    // fdec: decrement (--)
    public static <T extends Number> double fdec(T value) {
        return value.doubleValue() - 1;
    }
}
