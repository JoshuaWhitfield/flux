package functional.iterators;

import java.util.*;
import java.util.function.Function;
import functional.models.IterNode;
import functional.models.IterEdge;
import functional.models.TailCall;

public class FIterator<T, R> {
    private final IterNode node;
    private final IterEdge<Integer> index;
    private final IterEdge<Optional<T>> elem;
    private final IterEdge<Integer> interval;
    private final IterEdge<List<T>> body;
    private final IterEdge<List<R>> history;
    private final IterEdge<Boolean> back;
    private final IterEdge<Boolean> end;
    private final IterEdge<Boolean> cont;

    public FIterator(
        IterNode node,
        IterEdge<Integer> index,
        IterEdge<Optional<T>> elem,
        IterEdge<Integer> interval,
        IterEdge<List<T>> body,
        IterEdge<List<R>> history,
        IterEdge<Boolean> back,
        IterEdge<Boolean> end,
        IterEdge<Boolean> cont

    ) {
        this.node = node;
        this.index = index;
        this.elem = elem;
        this.interval = interval;
        this.body = body;
        this.history = history;
        this.back = back;
        this.end = end;
        this.cont = cont;
    }

    private R iterationAction(Function<IterNode, R> actionFn) {
        return actionFn.apply(node);
    }

    private boolean iterate() {
        int next = index.get() + interval.get();
        if (next >= body.get().size()) {
            end.set(true);
            return false;
        }

        index.set(next);
        elem.set(Optional.ofNullable(body.get().get(index.get())));
        return true;
    }

    private boolean backtrack() {
        int prev = index.get() - interval.get();
        if (prev > 0) {
            back.set(false);
            return false;
        }

        index.set(prev);
        elem.set(Optional.ofNullable(body.get().get(index.get())));
        back.set(false);
        return true;
    }

    public TailCall<List<R>> iterating(Function<IterNode, R> actionFn) {
        return new TailCall<List<R>>(() -> {
            if (back.get()) {
                return backtracking(actionFn).tco_run();
            }

            if (end.get()) return history.get();

            boolean valid = iterate();
            if (!valid) return history.get();

            List<R> _history = new ArrayList<>(history.get());
            elem.get().ifPresent(e -> _history.add(iterationAction(actionFn)));
            history.set(_history);

            return iterating(actionFn).tco_run();
        });
    }

    public TailCall<List<R>> backtracking(Function<IterNode, R> actionFn) {
    return new TailCall<List<R>>(() -> {
        boolean valid = backtrack();
        if (!valid) return history.get();

        List<R> _history = new ArrayList<>(history.get());
        elem.get().ifPresent(e -> _history.add(iterationAction(actionFn)));
        history.set(_history);

        // recursive call
        TailCall<List<R>> recursiveCall = iterating(actionFn);
        return recursiveCall.tco_run();
    });
}

}
