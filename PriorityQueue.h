#pragma once

#include <vector>
#include "HaffmanTree.h"

class PriorityQueue {
public:
    void Push(const std::pair<size_t, Haffman::HaffmanTree> el);

    std::pair<size_t, Haffman::HaffmanTree> Top();

    void Pop();

    size_t Size();

private:
    void Heapify(size_t i);

    std::vector<std::pair<size_t, Haffman::HaffmanTree> > heap_;
};
