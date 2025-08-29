package functional.models;

import java.util.Optional;

public final class Extractor {
    private Extractor() {} // no instances

    /**
     * Extracts an edge of type T from the node by key.
     * Returns Optional.empty() if missing or null.
     */
    @SuppressWarnings("unchecked")
    public static <T> Optional<T> extractEdge(IterNode node, String key) {
        IterEdge<?> edgeObj = node.getAs(key); // IterNode.get returns Object (the edge)
        if (edgeObj instanceof IterEdge<?>) {
            IterEdge<?> edge = (IterEdge<?>) edgeObj;
            Object value = edge.get();
            if (value == null) return Optional.empty();
            return Optional.of((T) value); // cast to T and wrap in Optional
        }
        return Optional.empty();
    }

}
