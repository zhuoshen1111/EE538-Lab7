#include "min_heap.hpp"

#include <stdexcept>
#include <utility>

namespace lab {

void MinHeap::build(const std::vector<int>& values) {
    // TODO: Replace data_, then sift down from the last internal node.
    // Be careful: an unsigned index cannot represent -1.
    data_=values;
    for (std::size_t i = data_.size()/2; i>0 ; --i){
        sift_down(i-1);
    
    }
}

void MinHeap::push(int value) {
    // TODO: Append, then sift up.
    data_.push_back(value);
    sift_up(data_.size()-1);

}

int MinHeap::pop() {
    // TODO: Check for an empty heap, save the root, replace it with the last
    // element, shrink the array, restore order, and return the saved value.
    if (data_.empty()){
        throw std::out_of_range("Heap is empty");
    }
    int minimum = data_[0];
    data_[0] = data_.back();
    data_.pop_back();

    if (!data_.empty()){
        sift_down(0);
    }
    return minimum;
}

int MinHeap::top() const {
    // TODO: Check for an empty heap, then read the root without changing it.
    if (data_.empty()){
        throw std::out_of_range("Heap is empty");

    }
    return data_[0];
    
}

void MinHeap::sift_up(std::size_t index) {
    // TODO: Compare with the parent while index > 0.
    while (index>0){
        std::size_t parent = (index-1)/2;

        if (data_[parent]<=data_[index]){
            break;
        }
        std::swap(data_[parent],data_[index]);
        index = parent;

    }
}

void MinHeap::sift_down(std::size_t index) {
    // TODO: Select the smaller existing child. Swap only when necessary.
    while (index < data_.size()){
        std::size_t left = 2* index +1;
        std::size_t right = 2* index +2;
        std::size_t smallest = index;

        if (left < data_.size() &&
            data_[left] < data_[smallest]){
                smallest=left;
            }
        if (right < data_.size() &&
            data_[right] < data_[smallest]) {
                smallest = right;
            }
        if (smallest == index) {
            break;
        }
        std::swap(data_[index],data_[smallest]);
        index = smallest;
    }
}

}// namespace lab
