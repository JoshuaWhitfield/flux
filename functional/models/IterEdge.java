package functional.models;

public class IterEdge<T> {
    private T value;

    public IterEdge(T value) { this.value = value; }

    public T get() { return value; }
    public void set(T value) { this.value = value; }
}