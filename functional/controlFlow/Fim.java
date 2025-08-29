package functional.controlFlow;

import java.util.function.Supplier;

public class Fim {

    @FunctionalInterface
    public interface ImperativeFunction<T, R> {
        R apply(T t);
    }
    
    public final <T, R> ImperativeFunction<T, R> fim(ImperativeFunction<T, R> actionFn, T args) {
        return (t) -> actionFn.apply(t);
    }

    public final <T> Supplier<T> fim(Supplier<T> actionFn) {
        return () -> actionFn.get();
    }

}