#include "min_heap.hpp"

#include <stdexcept>
#include <utility>

namespace lab {

void MinHeap::build(const std::vector<int>& values) {
    // TODO: Replace data_, then sift down from the last internal node.
    // Be careful: an unsigned index cannot represent -1.
    (void)values;
    throw std::logic_error("TODO: MinHeap::build");
}

void MinHeap::push(int value) {
    // TODO: Append, then sift up.
    (void)value;
    throw std::logic_error("TODO: MinHeap::push");
}

int MinHeap::pop() {
    // TODO: Check for an empty heap, save the root, replace it with the last
    // element, shrink the array, restore order, and return the saved value.
    throw std::logic_error("TODO: MinHeap::pop");
}

int MinHeap::top() const {
    // TODO: Check for an empty heap, then read the root without changing it.
    throw std::logic_error("TODO: MinHeap::top");
}

void MinHeap::sift_up(std::size_t index) {
    // TODO: Compare with the parent while index > 0.
    (void)index;
    throw std::logic_error("TODO: MinHeap::sift_up");
}

void MinHeap::sift_down(std::size_t index) {
    // TODO: Select the smaller existing child. Swap only when necessary.
    (void)index;
    throw std::logic_error("TODO: MinHeap::sift_down");
}

}  // namespace lab
