package functional.models;

import java.util.function.Supplier;

public class TailCall<T> {
    private final Supplier<T> func;

    public TailCall(Supplier<T> func) {
        this.func = func;
    }

    public T tco_run() {
        return func.get();
    }
}