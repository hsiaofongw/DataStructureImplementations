# 数据结构与算法实现演示 Demo of data structures and algorithms

## 优先队列 Priority Queue

### 定义 Definition

设类型 `KeyType` 满足全序要求，则我们可以定义定义了关于类型 `KeyType` 的下列操作的数据结构为类型 `KeyType` 的优先队列：

- `KeyType top(this) const` 获得队列最前的元素；
- `void pop(this)` 移除队列最前的元素；
- `void insert(this, const KeyType& element)` 将一个元素入列；
- `void clear(this)` 清空；
- `size_t size(this) const` 取队列长度；
- `bool empty(this) const` 空性判定；

### 堆 Heap

在当前版本，此项目已经实现了基于堆的优先队列。文件: `DataStructures/Heap.hpp`.

Until now, we have implemented a heap-based priority queue.

## 树 Tree

正如这个世界上没有两片一模一样的树叶，这个世界上的树的种类也是多种多样的，因此我们无法给「树」下一个放之四海而皆准的定义。

### 二叉搜索树 Binary Search Tree, i.e. BST.

在文件 `DataStructures/BinarySearchTree.hpp`，`BST::BSTHandle` 类定义并实现了一些有关于 BinarySearchTree 数据结构的操作。

File `DataStructures/BinarySearchTree.hpp` defined some operations on BST.

## 其他算法文件描述 Other Algorithm Files

- `Algorithms/IntersectionOfTwoLinkedList.hpp` 求两个链表的首个交点；Get first intersection point of two linked-list;
- `Algorithms/LongestPalindromeSubString.hpp` 求给定字符串的首个最长回文子串；Get first longest palindromic substring of given string;
- `Algorithms/ReverseLinkedList.hpp` 求给定链表的反转；Get the reverse of given linked-list;
- `Algorithms/AddStringFormBinary.hpp` 求给定的以字符串形式提供的二进制整数的整数加法结果的二进制表示的字符串形式；Get the string form of binary addition result in binary form of two binary form integer provided in string form;
- `Algorithms/TrapRainWater.hpp` 求「接雨水」问题的解，以及在代码注释中给出了关键数学公式和求解思路；Get the solution of the Trap Rain Water problem, and there are explanations in the comments in the code;
- `Algorithms/SubStringSearch.hpp` 求给定 text 字符串中是否出现了给定的 pattern 子串，如果 pattern 子串恰好在 text 中出现过，则返回其首次出现的下标，否则返回 text 作为 std::basic_string<char8_t> 的长度；Get the index of substring in a string;