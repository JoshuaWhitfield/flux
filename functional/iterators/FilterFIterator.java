package functional.iterators;

import functional.models.TailCall;
import java.util.*;
import java.util.function.BiFunction;

public class FilterFIterator<T, NodeT> {
    
    private int index;
    private T iterand;
    private final int interval;
    private final List<T> body;
    private List<T> history;
    private boolean end;

    public FilterFIterator(
        int startIndex,
        T firstIterand,
        int step,
        List<T> iterableBody
    ) {
        this.index = startIndex;
        this.iterand = firstIterand;
        this.interval = step;
        this.body = iterableBody;
        this.history = new ArrayList<>();
        this.end = false;
    }

    /**
     * Executes vallbavk and normalizes the result (returns Optional)
     */

    private <U> Optional<U> iterationAction(BiFunction<NodeT, T, U> actionFn, NodeT node)
    {
        U raw = actionFn.apply(node, iterand);
        return Optional.ofNullable(raw); // normalization: null -> empty Optional
    }

    /**
     * Move iterator forward
     */

    private boolean iterate() {
        index += interval;
        if (index >= body.size()) {
            end = true;
            return false;
        }
        
        iterand = body.get(index);
        return true;
    }

    /**
     * Main filtering logic
     */

    public <U> TailCall<List<U>> filtering(BiFunction<NodeT, T, U> actionFn, NodeT node)
    {
        return new TailCall<>(() -> {
            if (end) return (List<U>) history; // unchecked cast

            boolean valid = iterate();
            if (!valid) return (List<U>) history;

            Optional<U> normalized = iterationAction(actionFn, node);

            if (normalized.isPresent()) {
                // Add normalized value to history
                history.add((T) normalized.get()); // unsafe cast if U != T
            }

            return filtering(actionFn, node).tco_run();
        });
    }
    // Getters for iterand, index, history, etc.
    public T getIterand() { return iterand; }
    public int getIndex() { return index; }
    public List<T> getHistory() { return history; }
    public boolean isEnd() { return end; }
}


