//
// Created by 韦晓枫 on 2022/5/28.
//

#ifndef UNTITLED_HEAO_HEAP_HPP
#define UNTITLED_HEAO_HEAP_HPP

#include <iostream>
#include <concepts>
#include <memory>
#include <string>
#include <vector>
#include <functional>

template <typename T>
void printVector(const std::vector<T>& v) {
    if (v.empty()) {
        return;
    }

    auto it = std::begin(v);
    std::cout << *it;
    ++it;
    while (it != std::end(v)) {
        std::cout << ", " << *it;
        ++it;
    }
    std::cout << "\n";
}

template <typename T>
using Comparator = std::function<bool (const T& a, const T& b)>;

template <typename T>
class Heap {
public:
    /** 构造一个空堆，必须指定 key 的排序准则 */
    explicit Heap(const Comparator<T>& comparator);

    /** 通过堆化一个 std::vector<KeyType> 实例建立堆，并且使用传入的自定义比较器对各个 key 进行比较 */
    Heap(std::vector<T>&& heapStorageArray, const Comparator<T>& comparator);

    /** 不允许复制 */
    Heap(const Heap<T>& rhs) = delete;

    /** 声明了堆的移动语义，从而一个实现了移动语义的堆是可移的 */
    Heap(Heap<T>&& rhs) noexcept;

    /** 插入一个元素到堆中，并保证在元素被插入到堆的存储区域中之后此实例的堆性仍然得到维持 */
    void insert(const T& key);

    /** 查看堆顶部的元素 */
    T top() const;

    /** 弹出堆顶部堆元素，并且在弹出后立即着手进行堆性的恢复操作 */
    void pop();

    /** 清除堆的所有元素 */
    void clear();

    /** 检查这个堆是否是空堆，所谓空堆就是有 0 个元素的堆，并且我们约定认定空堆具备堆性 */
    [[nodiscard]] bool empty() const;

    /** 返回队列长度 */
    [[nodiscard]] size_t size() const;

    /** 更新比较器并且以新的比较器作为排序准则立即进行重新排序 */
    void updateComparator(const Comparator<T>& comparator);
private:
    /** 堆的存储区域，或者说是堆的 Array 形式 */
    std::vector<T> _store;

    /** 对堆的存储区域进行完整的检查，旨在检查堆性是否满足（只有调试的时候有用） */
    [[nodiscard]] bool isHeapPropertySatisfied();

    /** 尝试让 nodeOffset 指向的节点上浮，直至堆性恢复 */
    void reHeapifyByFloat(size_t nodeOffset);

    /** 尝试让 nodeOffset 指向的节点下沉，直至堆性恢复 */
    void reHeapifyBySink(size_t nodeOffset);

    /** 对堆的整个存储区域重新做一次完整的堆化操作，通常是需要在更改比较器之后立刻进行 */
    void fullReHeapify();

    /** 尝试获取一个节点的父节点，如果该节点的父节点不存在，则返回一个空指针 */
    std::unique_ptr<size_t> getParentOffset(size_t nodeOffset);

    /** 尝试获取一个节点的左子节点，如果该节点的左子节点不存在，则返回一个空指针 */
    std::unique_ptr<size_t> getLeftChildOffset(size_t nodeOffset);

    /** 尝试获取一个节点的右子节点，如果该节点的右子节点不存在，则返回一个空指针 */
    std::unique_ptr<size_t> getRightChildOffset(size_t nodeOffset);

    /** 交换两个节点的 key */
    void swap(size_t lhsOffset, size_t rhsOffset);

    /** 以 nodeOffset 指向的节点作为根，判断指定子堆的堆性是否被满足 */
    [[nodiscard]] bool _isHeapPropertySatisfied(size_t nodeOffset);

    /** 如果 nodeOffset 指向的节点大于等于它的所有子节点，则返回一个空指针，否则返回一个非空指针指向一个 size_t 对象 */
    std::unique_ptr<size_t> _findHeapPropertyViolation(size_t nodeOffset);

    /** 比较器，可被修改 */
    Comparator<T> comparator { [](const T& a, const T& b) { return true; } } ;

    /** 比较两个 node 的类型，返回 true 当前仅当表达式 lhs >= rhs 的求值结果为 true 或者可被隐式转化为 true */
    [[nodiscard]] bool compareGreaterThanOrEqual(size_t lhsNodeOffset, size_t rhsNodeOffset) const;
};


template <typename T>
Heap<T>::Heap(const Comparator<T>& _comparator)
: comparator(_comparator), _store(std::vector<T> {}) { }

template <typename T>
void Heap<T>::insert(const T &key) {
    this->_store.push_back(key);
    this->reHeapifyByFloat(0, this->_store.size());
}

template <typename T>
void Heap<T>::reHeapifyByFloat(size_t nodeOffset) {
    if (auto parentPtr = this->getParentOffset(nodeOffset)) {
        if (this->_findHeapPropertyViolation(*parentPtr)) {
            this->swap(*parentPtr, nodeOffset);
            this->reHeapifyByFloat(*parentPtr);
        }
    }
}

template <typename T>
std::unique_ptr<size_t> Heap<T>::getLeftChildOffset(size_t nodeOffset) {
    auto childOffset = nodeOffset * 2 + 1;
    if (childOffset < this->_store.size() && childOffset >= 0) {
        return std::make_unique<size_t>(childOffset);
    }

    return std::unique_ptr<size_t> {};
}

template <typename T>
std::unique_ptr<size_t> Heap<T>::getRightChildOffset(size_t nodeOffset) {
    auto childOffset = nodeOffset * 2 + 2;
    if (childOffset < this->_store.size() && childOffset >= 0) {
        return std::make_unique<size_t>(childOffset);
    }

    return std::unique_ptr<size_t> {};
}

template <typename T>
std::unique_ptr<size_t> Heap<T>::getParentOffset(size_t nodeOffset) {
    if (nodeOffset < 0 || nodeOffset >= this->_store.size()) {
        return std::unique_ptr<size_t> {};
    }

    size_t parentOffset = 0;
    if (nodeOffset % 2 == 0) {
        parentOffset = (nodeOffset - 2) / 2;
    } else {
        parentOffset = (nodeOffset - 1) / 2;
    }

    if (parentOffset >= 0 && parentOffset < this->_store.size()) {
        return std::make_unique<size_t>(parentOffset);
    }

    return std::unique_ptr<size_t> {};
}

template <typename T>
void Heap<T>::reHeapifyBySink(size_t nodeOffset) {
    if (this->_findHeapPropertyViolation(nodeOffset)) {
        if (auto leftPtr = this->getLeftChildOffset(nodeOffset)) {
            if (auto rightPtr = this->getRightChildOffset(nodeOffset)) {
                if (this->compareGreaterThanOrEqual(*leftPtr, *rightPtr)) {
                    this->swap(*leftPtr, nodeOffset);
                    this->reHeapifyBySink(*leftPtr);
                } else {
                    this->swap(*rightPtr, nodeOffset);
                    this->reHeapifyBySink(*rightPtr);
                }
            } else {
                this->swap(*leftPtr, nodeOffset);
                this->reHeapifyBySink(*leftPtr);
            }

        }
    }
}

template <typename T>
void Heap<T>::swap(size_t lhsOffset, size_t rhsOffset) {
    T temp = this->_store[lhsOffset];
    this->_store[lhsOffset] = this->_store[rhsOffset];
    this->_store[rhsOffset] = temp;
}

template <typename T>
Heap<T>::Heap(Heap<T> &&rhs) noexcept : _store(std::move(rhs._store)) { }

template <typename T>
T Heap<T>::top() const {
    return static_cast<T>(this->_store[0]);
}

template <typename T>
void Heap<T>::pop() {
    if (this->_store.empty()) {
        return;
    }

    this->swap(0, this->_store.size()-1);
    this->_store.pop_back();
    if (!this->_store.empty()) {
        this->reHeapifyBySink(0);
    }
}

template <typename T>
bool Heap<T>::empty() const {
    return this->_store.empty();
}

template <typename T>
bool Heap<T>::isHeapPropertySatisfied() {
    return this->_isHeapPropertySatisfied(0);
}

template <typename T>
bool Heap<T>::_isHeapPropertySatisfied(size_t nodeOffset) {
    if (this->_findHeapPropertyViolation(nodeOffset)) {
        return false;
    }

    if (auto leftPtr = this->getLeftChildOffset(nodeOffset)) {
        if (!this->_isHeapPropertySatisfied(*leftPtr)) {
            return false;
        }
    }

    if (auto rightPtr = this->getRightChildOffset(nodeOffset)) {
        if (!this->_isHeapPropertySatisfied(*rightPtr)) {
            return false;
        }
    }

    return true;
}

template <typename T>
std::unique_ptr<size_t> Heap<T>::_findHeapPropertyViolation(size_t nodeOffset) {
    auto emptyPtr = std::unique_ptr<size_t> {};
    if (nodeOffset >= this->_store.size()) {
        return emptyPtr;
    }

    if (auto leftPtr = this->getLeftChildOffset(nodeOffset)) {
        if (!this->compareGreaterThanOrEqual(nodeOffset, *leftPtr)) {
            return std::move(leftPtr);
        }
    }

    if (auto rightPtr = this->getRightChildOffset(nodeOffset)) {
        if (!this->compareGreaterThanOrEqual(nodeOffset, *rightPtr)) {
            return std::move(rightPtr);
        }
    }

    return emptyPtr;
}

template <typename T>
bool Heap<T>::compareGreaterThanOrEqual(size_t lhsNodeOffset, size_t rhsNodeOffset) const {
    return this->comparator(this->_store[lhsNodeOffset], this->_store[rhsNodeOffset]);
}

template <typename T>
void Heap<T>::fullReHeapify() {
    if (!this->_store.empty()) {
        for (size_t ptr = 0; ptr < this->_store.size(); ++ptr) {
            this->reHeapifyByFloat(ptr+1);
        }
    }
}

template <typename T>
Heap<T>::Heap(std::vector<T> &&heapStorageArray, const Comparator<T> &_comparator)
: _store(std::move(heapStorageArray)), comparator(_comparator) {
    this->fullReHeapify();
}

template <typename T>
void Heap<T>::updateComparator(const Comparator<T> &_comparator) {
    this->comparator = _comparator;
    this->fullReHeapify();
}

template <typename T>
void Heap<T>::clear() {
    this->_store.clear();
}

template <typename T>
size_t Heap<T>::size() const {
    return this->_store.size();
}

#endif //UNTITLED_HEAO_HEAP_HPP
