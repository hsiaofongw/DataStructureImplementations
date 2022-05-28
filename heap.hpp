//
// Created by 韦晓枫 on 2022/5/28.
//

#ifndef UNTITLED_HEAO_HEAP_HPP
#define UNTITLED_HEAO_HEAP_HPP

#include <concepts>
#include <memory>
#include <string>
#include <vector>


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


template<typename T>
concept Comparable = requires(T a, T b) { a >= b; };

template <Comparable T>
class Heap {
public:
    /** 构造一个空堆 */
    Heap( );

    /** 通过堆化一个 std::vector<T> 实例建立堆  */
    explicit Heap(std::vector<T>&& heapStorageArray) noexcept;

    /** 不允许复制 */
    Heap(const Heap<T>& rhs) = delete;

    /** 实现堆的移动语义 */
    Heap(Heap<T>&& rhs) noexcept;

    /** 插入一个元素到堆中，并保证在元素被插入到堆的存储区域中之后此实例的堆性仍然得到维持 */
    void insert(const T& key);

    /** 查看堆顶部的元素 */
    T top();

    /** 弹出堆顶部堆元素，并且在弹出后立即恢复堆性 */
    void pop();

    [[nodiscard]] bool empty() const;

    /** 对堆的存储区域进行完整的检查，旨在检查堆性是否满足 */
    [[nodiscard]] bool isHeapPropertySatisfied();

    /** 获得对存储空间对只读引用 */
    const std::vector<T>& getStorage();
private:
    std::vector<T> _store;

    /** 尝试让 nodeOffset 指向的节点上浮 */
    void reHeapifyByFloat(size_t nodeOffset);

    /** 尝试让 nodeOffset 指向的节点下沉 */
    void reHeapifyBySink(size_t nodeOffset);

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
};


template<Comparable T>
Heap<T>::Heap() : _store(std::vector<T> {}) { }

template<Comparable T>
void Heap<T>::insert(const T &key) {
    this->_store.push_back(key);
    this->reHeapifyByFloat(0, this->_store.size());
}

template<Comparable T>
void Heap<T>::reHeapifyByFloat(size_t nodeOffset) {
    if (auto parentPtr = this->getParentOffset(nodeOffset)) {
        if (this->_findHeapPropertyViolation(*parentPtr)) {
            this->swap(*parentPtr, nodeOffset);
            this->reHeapifyByFloat(*parentPtr);
        }
    }
}

template<Comparable T>
std::unique_ptr<size_t> Heap<T>::getLeftChildOffset(size_t nodeOffset) {
    auto childOffset = nodeOffset * 2 + 1;
    if (childOffset < this->_store.size() && childOffset >= 0) {
        return std::make_unique<size_t>(childOffset);
    }

    return std::unique_ptr<size_t> {};
}

template<Comparable T>
std::unique_ptr<size_t> Heap<T>::getRightChildOffset(size_t nodeOffset) {
    auto childOffset = nodeOffset * 2 + 2;
    if (childOffset < this->_store.size() && childOffset >= 0) {
        return std::make_unique<size_t>(childOffset);
    }

    return std::unique_ptr<size_t> {};
}

template<Comparable T>
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

template<Comparable T>
void Heap<T>::reHeapifyBySink(size_t nodeOffset) {
    if (this->_findHeapPropertyViolation(nodeOffset)) {
        if (auto leftPtr = this->getLeftChildOffset(nodeOffset)) {
            if (auto rightPtr = this->getRightChildOffset(nodeOffset)) {
                T lKey = this->_store[*leftPtr];
                T rKey = this->_store[*rightPtr];
                if (lKey >= rKey) {
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

template<Comparable T>
void Heap<T>::swap(size_t lhsOffset, size_t rhsOffset) {
    T temp = this->_store[lhsOffset];
    this->_store[lhsOffset] = this->_store[rhsOffset];
    this->_store[rhsOffset] = temp;
}

template<Comparable T>
Heap<T>::Heap(std::vector<T> &&heapStorageArray) noexcept : _store(std::move(heapStorageArray)) {
    if (!this->_store.empty()) {
        for (size_t ptr = 0; ptr < this->_store.size(); ++ptr) {
            this->reHeapifyByFloat(ptr+1);
        }
    }
}

template<Comparable T>
Heap<T>::Heap(Heap<T> &&rhs) noexcept : _store(std::move(rhs._store)) { }

template<Comparable T>
T Heap<T>::top() {
    return static_cast<T>(this->_store[0]);
}

template<Comparable T>
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

template<Comparable T>
bool Heap<T>::empty() const {
    return this->_store.empty();
}

template<Comparable T>
bool Heap<T>::isHeapPropertySatisfied() {
    return this->_isHeapPropertySatisfied(0);
}

template<Comparable T>
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

template<Comparable T>
std::unique_ptr<size_t> Heap<T>::_findHeapPropertyViolation(size_t nodeOffset) {
    if (nodeOffset >= this->_store.size()) {
        return std::unique_ptr<size_t> {};
    }

    T currentKey = this->_store[nodeOffset];

    if (auto leftPtr = this->getLeftChildOffset(nodeOffset)) {
        T leftKey = this->_store[*leftPtr];
        if (currentKey >= leftKey) {

        } else {
            return std::move(leftPtr);
        }
    }

    if (auto rightPtr = this->getRightChildOffset(nodeOffset)) {
        T rightKey = this->_store[*rightPtr];
        if (currentKey >= rightKey) {

        } else {
            return std::move(rightPtr);
        }
    }

    return std::unique_ptr<size_t> {};
}

template<Comparable T>
const std::vector<T> &Heap<T>::getStorage() {
    return this->_store;
}


#endif //UNTITLED_HEAO_HEAP_HPP
