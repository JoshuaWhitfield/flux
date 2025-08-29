package functional.controlFlow;

public final class Callables {
    private Callables() {} // prevent instantiation

    @FunctionalInterface
    public interface VoidCallable { void call(); }

    @FunctionalInterface
    public interface BoolCallable { boolean call(); }

    @FunctionalInterface
    public interface AnyCallable<T> { T call(); }
}