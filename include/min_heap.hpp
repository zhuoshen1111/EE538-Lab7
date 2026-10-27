#ifndef EE538_MIN_HEAP_HPP
#define EE538_MIN_HEAP_HPP

#include <cstddef>
#include <vector>

namespace lab {

// Array-backed min-heap. Equal values and negative values are allowed.
class MinHeap {
public:
    // Replace the current contents. Use bottom-up sift_down in O(n) time.
    void build(const std::vector<int>& values);

    // Append the value, then restore heap order in O(log n) time.
    void push(int value);

    // Remove and return the smallest value in O(log n) time.
    // Throw std::out_of_range if the heap is empty.
    int pop();

    // Return the smallest value without removing it, in O(1) time.
    // Throw std::out_of_range if the heap is empty.
    int top() const;

    bool empty() const noexcept { return data_.empty(); }
    std::size_t size() const noexcept { return data_.size(); }

    // Read-only access for inspecting the array and checking the heap property.
    const std::vector<int>& data() const noexcept { return data_; }

private:
    std::vector<int> data_;
    void sift_up(std::size_t index);
    void sift_down(std::size_t index);
};

}  // namespace lab

#endif
