//
// Created by 韦晓枫 on 2022/5/28.
//

#include "heap.hpp"
#include <string>

template<Comparable T>
Heap<T>::Heap() : _store(std::vector<T> {}) { }

template<Comparable T>
void Heap<T>::insert(const T &key) {
    this->_store.push_back(key);
    this->reHeapifyByFloat(0, this->_store.size());
}

template<Comparable T>
void Heap<T>::reHeapifyByFloat(size_t begin, size_t end) {
    if (begin == end) {
        return;
    }

    auto lastNodeOffset = end - 1;
    auto parentOffset = this->getParentOffset(lastNodeOffset);
    Comparable auto parentKey = static_cast<T>(this->_store[parentOffset]);
    Comparable auto childKey = static_cast<T>(this->_store[lastNodeOffset]);
    // 检查新节点加入后堆性是否仍然满足
    if (parentKey >= childKey) {
        return;
    }

    this->swap(lastNodeOffset, parentOffset);
    this->reHeapifyByFloat(0, parentOffset + 1);
}

template<Comparable T>
size_t Heap<T>::getLeftChildOffset(size_t nodeOffset) {
    return nodeOffset * 2 + 1;
}

template<Comparable T>
size_t Heap<T>::getRightChildOffset(size_t nodeOffset) {
    return nodeOffset * 2 + 2;
}

template<Comparable T>
size_t Heap<T>::getParentOffset(size_t nodeOffset) {
    if (nodeOffset % 2 == 0) {
        return (nodeOffset - 2) / 2;
    } else {
        return (nodeOffset - 1) / 2;
    }
}

template<Comparable T>
void Heap<T>::reHeapifyBySink(size_t nodeOffset) {
    if (nodeOffset >= this->_store.size()) {
        return;
    }

    Comparable auto topKey = static_cast<T>(this->_store[nodeOffset]);
    auto leftChildOffset = this->getLeftChildOffset(nodeOffset);
    auto rightChildOffset = this->getRightChildOffset(nodeOffset);
    Comparable auto leftChildKey = static_cast<T>(this->_store[leftChildOffset]);
    Comparable auto rightChildKey = static_cast<T>(this->_store[rightChildOffset]);
    if (topKey >= leftChildKey && topKey >= rightChildKey) {
        // 堆性满足
        return;
    }

    this->swap(nodeOffset, leftChildOffset);
    this->reHeapifyBySink(leftChildOffset);
}

template<Comparable T>
void Heap<T>::swap(size_t lhsOffset, size_t rhsOffset) {
    Comparable auto temp = this->_store[lhsOffset];
    this->_store[lhsOffset] = this->_store[rhsOffset];
    this->_store[rhsOffset] = temp;
}

template<Comparable T>
Heap<T>::Heap(std::vector<T> &&heapStorageArray) noexcept : _store(std::move(heapStorageArray)) {
    if (!this->_store.empty()) {
        for (size_t ptr = 0; ptr < this->_store.size(); ++ptr) {
            this->reHeapifyByFloat(0, ptr+1);
        }
    }
}

template<Comparable T>
Heap<T>::Heap(Heap<T> &&rhs) noexcept : _store(std::move(rhs._store)) { }

template<Comparable T>
const std::vector<T> &Heap<T>::getStorage() {
    return this->_store;
}

template<Comparable T>
const T &Heap<T>::top() {
    return static_cast<T>(this->_store.at(0));
}

template<Comparable T>
void Heap<T>::pop() {
    if (this->_store.empty()) {
        return;
    }

    this->swap(0, this->_store.size()-1);
    this->_store.pop_back();
    this->reHeapifyBySink(0);
}
