# 数据结构与算法实现演示

## 优先队列

### 定义

设类型 `KeyType` 满足全序要求，则我们可以定义定义了关于类型 `KeyType` 的下列操作的数据结构为类型 `KeyType` 的优先队列：

- `KeyType top(this) const` 获得队列最前的元素；
- `void pop(this)` 移除队列最前的元素；
- `void insert(this, const KeyType& element)` 将一个元素入列；
- `void clear(this)` 清空；
- `size_t size(this) const` 取队列长度；
- `bool empty(this) const` 空性判定；

### 堆

在当前版本，此项目已经实现了基于堆的优先队列。文件: `DataStructures/Heap.hpp`.

## 树

正如这个世界上没有两片一模一样的树叶，这个世界上的树的种类也是多种多样的，因此我们无法给「树」下一个放之四海而皆准的定义。

### 二叉搜索树

在文件 `DataStructures/BinarySearchTree.hpp`，`BST::BSTHandle` 类定义并实现了一些有关于 BinarySearchTree 数据结构的操作。

## 其他算法文件描述

- `Algorithms/IntersectionOfTwoLinkedList.hpp` 求两个链表的首个交点；
- `Algorithms/LongestPalindromeSubString.hpp` 求给定字符串的首个最长回文子串；
- `Algorithms/ReverseLinkedList.hpp` 求给定链表的反转
- `Algorithms/AddStringFormBinary.hpp` 求给定的以字符串形式提供的二进制整数的整数加法结果的二进制表示的字符串形式；