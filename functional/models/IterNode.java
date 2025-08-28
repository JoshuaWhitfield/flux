package functional.models;

import java.util.HashMap;
import java.util.Map;

public class IterNode {
    private final Map<String, Object> edges = new HashMap<>();

    public <T> void put(String key, IterEdge<T> edge) {
        edges.put(key, edge);
    }

    @SuppressWarnings("unchecked")
    public <T> IterEdge<T> getAs(String key) {
        return (IterEdge<T>) edges.get(key);
    }
}