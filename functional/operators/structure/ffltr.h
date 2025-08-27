#ifndef FFLTR_H
#define FFLTR_H

#include <vector>
#include <functional>
#include "../../iterators/filter_fit.h"
#include "../../models/node.h"

// ffltr: declarative wrapper for FilterIterator
template <typename T, typename ActionFn>
std::vector<T> ffltr(std::vector<T> input, ActionFn actionFn) {
    if (input.empty()) return {};

    FilterIterator<T, Node> filter_iter(-1, T{}, 1, input);

    Node node = createNode();  // always use Node
    return filter_iter.filtering(actionFn, node).run();
}




#endif // FFLTR_H
