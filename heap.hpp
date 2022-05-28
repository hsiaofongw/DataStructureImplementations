//
// Created by 韦晓枫 on 2022/5/28.
//

#ifndef UNTITLED_HEAO_HEAP_HPP
#define UNTITLED_HEAO_HEAP_HPP


#include <concepts>
#include <memory>
#include <string>
#include <vector>

template<typename T>
concept Comparable = requires(T a, T b) { a >= b; };

template <Comparable T>
class Heap {
public:
    /** 构造一个空堆 */
    Heap( );

    /** 通过堆化一个 std::vector<T> 实例建立堆  */
    Heap(std::vector<T>&& heapStorageArray) noexcept;

    /** 不允许复制 */
    Heap(const Heap<T>& rhs) = delete;

    /** 实现堆的移动语义 */
    Heap(Heap<T>&& rhs) noexcept;

    /** 插入一个元素到堆中，并保证在元素被插入到堆的存储区域中之后此实例的堆性仍然得到维持 */
    void insert(const T& key);

    /** 查看堆顶部的元素 */
    const T& top();

    /** 弹出堆顶部堆元素，并且在弹出后立即恢复堆性 */
    void pop();

    /** 获取一个内部存储的只读引用 */
    const std::vector<T>& getStorage();
private:
    std::vector<T> _store;

    void reHeapifyByFloat(size_t begin, size_t end);

    void reHeapifyBySink(size_t nodeOffset);

    size_t getParentOffset(size_t nodeOffset);

    size_t getLeftChildOffset(size_t nodeOffset);

    size_t getRightChildOffset(size_t nodeOffset);

    void swap(size_t lhsOffset, size_t rhsOffset);
};

#endif //UNTITLED_HEAO_HEAP_HPP
