package functional.control_flow;

import java.util.*;
import java.util.function.BooleanSupplier;
import java.util.function.Function;
import java.util.function.Supplier;

import functional.models.IterEdge;
import functional.models.IterNode;
import functional.iterators.FIterator;
// import functional.control_flow.Bincase; unnecessary import

public class FFor {
    public static <T, R> List<R> ffor(
        List<T> iterable,
        Function<IterNode, R> actionFn,
        int startAt,
        int jump
    ) {
        IterNode node = new IterNode();

        IterEdge<Integer> index = new IterEdge<>(startAt);
        IterEdge<Optional<T>> elem = new IterEdge<>(Optional.empty());
        IterEdge<Integer> interval = new IterEdge<>(jump);
        IterEdge<List<T>> body = new IterEdge<>(iterable);
        IterEdge<List<R>> history = new IterEdge<>(new ArrayList<>());
        IterEdge<Boolean> back = new IterEdge<>(false);
        IterEdge<Boolean> end = new IterEdge<>(false);
        IterEdge<Boolean> cont = new IterEdge<>(false);
    
        node.put("index", index);
        node.put("elem", elem);
        node.put("interval", interval);
        node.put("body", body);
        node.put("history", history);
        node.put("back", back);
        node.put("end", end);
        node.put("cont", cont);

        FIterator<T, R> iterator = new FIterator<T, R>(
            node, index, elem,
            interval, body, history,
            back, end, cont
        );
        
        return Bincase.bincase(
            (BooleanSupplier) () -> { return end.get(); },
            (Supplier<List<R>>) () -> iterator.iterating(actionFn).tco_run(),
            (Supplier<List<R>>) () -> history.get()
        );
    }
}