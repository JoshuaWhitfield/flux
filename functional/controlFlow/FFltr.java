package functional.controlFlow;

import functional.iterators.FilterFIterator;
import java.util.*;
import java.util.function.BiFunction;

public final class FFltr {
    private FFltr() {} // prevent instantiation

    // ffltr: declarative wrapper for FilterFIterator
    public static <T, NodeT> List<T> ffltr(
        List<T> input,
        BiFunction<NodeT, T, T> actionFn,
        NodeT nodeFactory
    ) {
        if (input.isEmpty()) return Collections.emptyList();

        // Always start same as your C++: (-1, T{}, 1, input)
        FilterFIterator<T, NodeT> filterIter =
            new FilterFIterator<>(-1, null, 1, input);

        // nodeFactory: equivalent to createNode()
        return filterIter.filtering(actionFn, nodeFactory).tco_run();
    }
}
