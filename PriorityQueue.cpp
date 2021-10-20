#include <cstddef>
#include "PriorityQueue.h"

std::pair<size_t, Haffman::HaffmanTree> PriorityQueue::Top() {
    if (heap_.empty()) {
        throw std::out_of_range("Trying to get element while heap is empty");
    }
    return heap_[0];
}

void PriorityQueue::Pop() {
    if (heap_.empty()) {
        throw std::out_of_range("Trying to delete element while heap is empty");
    }
    std::swap(heap_[0], heap_[heap_.size() - 1]);
    heap_.erase(heap_.end() - 1);
    Heapify(1);
}

size_t PriorityQueue::Size() {
    return heap_.size();
}

size_t GetParent(size_t i) {
    if (i == 0) {
        return 0;
    }
    return (i - 1) / 2;
}

std::pair<size_t, size_t> GetChildren(size_t i) {
    return {i * 2 + 1, i * 2 + 2};
}

void PriorityQueue::Push(const std::pair<size_t, Haffman::HaffmanTree> el) {
    heap_.push_back(el);
    size_t i = heap_.size() - 1;
    size_t parent = GetParent(i);
    while (i > 0 && heap_[parent] > heap_[i]) {
        std::swap(heap_[parent], heap_[i]);
        i = parent;
        parent = GetParent(i);
    }
}

void PriorityQueue::Heapify(size_t i) {
    while (true) {
        auto children = GetChildren(i);
        size_t min_child = i;
        if (children.first < heap_.size() && heap_[children.first] < heap_[min_child]) {
            min_child = children.first;
        }
        if (children.second < heap_.size() && heap_[children.second] < heap_[min_child]) {
            min_child = children.first;
        }
        if (min_child == i) {
            break;
        }
        std::swap(heap_[i], heap_[min_child]);
        i = min_child;
    }
}
