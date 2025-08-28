package functional.control_flow;

import java.util.*;
import java.util.function.BiFunction;

import functional.iterators.FilterFIterator;
import functional.models.TailCall;

/**
 * Functional Filter wrapper - simplifies FilterFIterator
 */

public class FFilter {
    
    /**
     * Filters a list using a functional callback.
     * 
     * @param iterable  The list to filter
     * @param actionFn  The filtering function (returns value or null to skip)
     * @param <T>       Element type
     * @param <NodeT>   Node context type (can be Object if not used)
     * @return TailCall For TCO-safe recursive filtering
     */

    public static <T, NodeT> TailCall<List<T>> ffilter(
        List<T> iterable,
        BiFunction<NodeT, T, T> actionFn,
        NodeT nodeContext
    ) {
        if (iterable.isEmpty()) {
            return new TailCall<>(() -> Collections.emptyList());
        }

        FilterFIterator<T, NodeT> iterator = new FilterFIterator<T,NodeT>(
            0, iterable.get(0), 1, iterable
        );

        return iterator.filtering(actionFn, nodeContext);
    }

}
