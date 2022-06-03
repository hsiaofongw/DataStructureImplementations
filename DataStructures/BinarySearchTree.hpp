//
// Created by 韦晓枫 on 2022/5/30.
//

#ifndef PRIORITYQUEUEIMPLEMENTATIONS_BINARYSEARCHTREE_HPP
#define PRIORITYQUEUEIMPLEMENTATIONS_BINARYSEARCHTREE_HPP

#include <concepts>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <deque>

namespace BST {

    template<typename T>
    concept Comparable = requires(const T& a, const T& b)
    {
        a > b; a == b; a < b;  // Require that below expressions: a > b, a == b, and a < b all compiles.
    };

    template <Comparable KeyType>
    using KeyPtr = std::shared_ptr<KeyType>;

    template <typename ValueType>
    using ValuePtr = std::shared_ptr<ValueType>;

    template <Comparable KeyType, typename ValueType>
    struct BSTNode;

    template <Comparable KeyType, typename ValueType>
    using NodePtr = std::shared_ptr<BSTNode<KeyType, ValueType>>;

    template <Comparable KeyType, typename ValueType>
    struct BSTNode {
        /** 指向 KeyType 实例 */
        KeyPtr<KeyType> keyPtr;

        /** 指向 ValueType 实例 */
        ValuePtr<ValueType> valuePtr;

        /** 指向左子树 */
        NodePtr<KeyType, ValueType> leftPtr;

        /** 指向右子树 */
        NodePtr<KeyType, ValueType> rightPtr;
    };

    /** 返回一个指向空节点的指针 */
    template <Comparable KeyType, typename ValueType>
    decltype(auto) makeEmptyNode() {
        return std::make_shared<BSTNode<KeyType, ValueType>>();
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> updateBoundary(
            NodePtr<KeyType, ValueType> current,
            NodePtr<KeyType, ValueType> candidate,
            KeyPtr<KeyType> keyPtr,
            std::function<NodePtr<KeyType, ValueType> (NodePtr<KeyType, ValueType>)> tryBetterGuess,
            std::function<NodePtr<KeyType, ValueType> (NodePtr<KeyType, ValueType>)> tryWorseGuess,
            std::function<bool (const KeyType& currentKey, const KeyType& candidateKey)> outOfCandidateCriterion,
            std::function<bool (const KeyType& currentKey, const KeyType& upperBound)> outOfBoundCriterion
    ) {
        if (!current) {
            return candidate;
        }

        const KeyType& currentKey = *current->keyPtr;
        const KeyType& upperBound = *keyPtr;

        if (!candidate) {
            // there is no candidate yet.
            if (outOfBoundCriterion(currentKey, upperBound)) {
                return updateBoundary(tryWorseGuess(current), candidate, keyPtr, tryBetterGuess, tryWorseGuess, outOfCandidateCriterion, outOfBoundCriterion);
            }
            else {
                return updateBoundary(current, current, keyPtr, tryBetterGuess, tryWorseGuess, outOfCandidateCriterion, outOfBoundCriterion);
            }
        }

        const KeyType& candidateKey = *candidate->keyPtr;

        if (outOfCandidateCriterion(currentKey, candidateKey)) {
            // currentKey in (-infinity, candidateKey]
            return updateBoundary(tryBetterGuess(current), candidate, keyPtr, tryBetterGuess, tryWorseGuess, outOfCandidateCriterion, outOfBoundCriterion);
        }
        else if (outOfBoundCriterion(currentKey, upperBound)) {
            // currentKey in [upperBound, +infinity)
            return updateBoundary(tryWorseGuess(current), candidate, keyPtr, tryBetterGuess, tryWorseGuess, outOfCandidateCriterion, outOfBoundCriterion);
        } else {
            // currentKey in (candidateKey, upperBound)
            return updateBoundary(tryBetterGuess(current), current, keyPtr, tryBetterGuess, tryWorseGuess, outOfCandidateCriterion, outOfBoundCriterion);
        }
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> updateFloorCandidate(NodePtr<KeyType, ValueType> current, NodePtr<KeyType, ValueType> candidate, KeyPtr<KeyType> keyPtr) {
        return updateBoundary<KeyType, ValueType>(
                current,
                candidate,
                keyPtr,
                [](NodePtr<KeyType, ValueType> nodePtr) { return nodePtr->rightPtr; },
                [](NodePtr<KeyType, ValueType> nodePtr) { return nodePtr->leftPtr; },
                [](const KeyType& currentKey, const KeyType& candidateKey) { return currentKey < candidateKey || currentKey == candidateKey; },
                [](const KeyType& currentKey, const KeyType& boundKey) { return currentKey > boundKey || currentKey == boundKey; }
        );
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> updateCeilCandidate(NodePtr<KeyType, ValueType> current, NodePtr<KeyType, ValueType> candidate, KeyPtr<KeyType> keyPtr) {
        return updateBoundary<KeyType, ValueType>(
                current,
                candidate,
                keyPtr,
                [](NodePtr<KeyType, ValueType> nodePtr) { return nodePtr->leftPtr; },
                [](NodePtr<KeyType, ValueType> nodePtr) { return nodePtr->rightPtr; },
                [](const KeyType& currentKey, const KeyType& candidateKey) { return currentKey > candidateKey || currentKey == candidateKey; },
                [](const KeyType& currentKey, const KeyType& boundKey) { return currentKey < boundKey || currentKey == boundKey; }
        );
    }

    /** 二叉搜索树句柄类，一个 ::BST::BSTHandle 实例可以用来操纵一个 ::BST::BSTNode 实例 */
    template<Comparable KeyType, typename ValueType>
    class BSTHandle {

    using NodePtr = BST::NodePtr<KeyType, ValueType>;
    using KeyPtr = BST::KeyPtr<KeyType>;
    using ValuePtr = BST::ValuePtr<ValueType>;
    using Handle = BST::BSTHandle<KeyType, ValueType>;

    public:
        /** 默认构造一个 BSTHandle 实例，该实例控制一个 size = 0 的树 */
        BSTHandle( );

        /** 构造一个 BSTHandle 实例，该实例控制一个指定的树（要求传入一个 NodePtr）*/
        explicit BSTHandle(NodePtr nodePtr);

        /** 计算一棵树有多少个非空节点 */
        static size_t size(const NodePtr& nodePtr);

        /** 插入一个键值对记录到一个指定的树中 */
        static void insert(NodePtr& nodePtr, const KeyPtr &keyPtr, const ValuePtr &valuePtr);

        /** 搜索一个树，看有没有特定的键对应的值，如果没有返回一个空指针，如果有返回一个指针指向那个值 */
        static BST::ValuePtr<ValueType> search(const NodePtr& nodePtr, const KeyPtr &keyPtr);

        /** 返回最小的键对应的树的节点 */
        static BST::NodePtr<KeyType, ValueType> min(const NodePtr& nodePtr);

        /** 返回最大的键对应的树的节点 */
        static BST::NodePtr<KeyType, ValueType> max(const NodePtr& nodePtr);

        /** 删除最小的键对应的树的节点 */
        static void deleteMin(NodePtr& nodePtr);

        /** 删除最大的键对应的树的节点 */
        static void deleteMax(NodePtr& nodePtr);

        /** In-Order, Recursive Left, Current, Then Recursive Right */
        static void traverseInOrderLNR(
                const NodePtr &root,
                std::function<void(const NodePtr &)> fn,
                std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
                std::function<bool(const NodePtr &)> probePredicate
        );

        /** In-Order, Recursive Right, Current, Then Recursive Left */
        static void traverseInOrderRNL(
                const NodePtr &root,
                std::function<void(const NodePtr &)> fn,
                std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
                std::function<bool(const NodePtr &)> probePredicate
        );

        /** 对 key 下取整，返回那个 key 对应的最大的不超过它的 key 对应的节点指针 */
        static BST::NodePtr<KeyType, ValueType> floor(const NodePtr& currentNodePtr, const KeyPtr &keyPtr);

        /** 对 key 上取整，返回那个 key 对应的最大的不小于它的 key 对应的节点指针 */
        static BST::NodePtr<KeyType, ValueType> ceil(const NodePtr& currentNodePtr, const KeyPtr &keyPtr);

        static std::unique_ptr<std::vector<BST::NodePtr<KeyType, ValueType>>> rangeSearchMany(
                const NodePtr& root,
                const KeyType& lowerBound,
                const KeyType& upperBound
        );

        static NodePtr
        rangeSearchOne(const NodePtr& root, const KeyType& lowerBound, const KeyType& upperBound);

        static void deleteKey(NodePtr& root, const KeyType& key);

        std::unique_ptr<std::vector<BST::NodePtr<KeyType, ValueType>>> rangeSearchMany(
                const KeyType& lowerBound,
                const KeyType& upperBound
        );

        NodePtr get();

        [[nodiscard]] size_t size() const;

        void insert(const KeyPtr& keyPtr, const ValuePtr& valuePtr);

        BST::ValuePtr<ValueType> search(const KeyPtr &keyPtr);

        BST::NodePtr<KeyType, ValueType> min() const;

        BST::NodePtr<KeyType, ValueType> max() const;

        /** 删除这个句柄控制的树的 key 最小的节点 */
        void deleteMin();

        /** 删除这个句柄控制的树的 key 最大的节点 */
        void deleteMax();

        /** 判断这个句柄是否控制的是一颗没有任何元素的树 */
        [[nodiscard]] bool empty() const;

        /** 让这个句柄控制另外一个 ::BST::BSTNode, 实现复用一个句柄实例的同时操纵多个 ::BST::BSTNode 的需求 */
        void reset(NodePtr nodePtr);

        BST::NodePtr<KeyType, ValueType> floor(const KeyPtr &keyPtr) const;

        BST::NodePtr<KeyType, ValueType> ceil(const KeyPtr &keyPtr) const;

        /** In-Order, Recursive Left, Current, Then Recursive Right */
        void traverseInOrderLNR(
                std::function<void(const NodePtr &)> fn,
                std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
                std::function<bool(const NodePtr &)> probePredicate
        );

        /** In-Order, Recursive Right, Current, Then Recursive Left */
        void traverseInOrderRNL(
                std::function<void(const NodePtr &)> fn,
                std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
                std::function<bool(const NodePtr &)> probePredicate
        );

    private:
        NodePtr nodePtr;

        static void deleteNodeInPlace(NodePtr& childRef);

        /** 把 b 指向的节点的第一层数据全部复制到 a */
        static void assignNodePtr(NodePtr& a, const NodePtr& b);

        /** 删除这个句柄控制的树的左子节点 */
        static void deleteLeftChild(NodePtr& root);

        /** 删除这个句柄控制的树的右子节点 */
        static void deleteRightChild(NodePtr& root);
    };

    template<Comparable KeyType, typename ValueType>
    BSTHandle<KeyType, ValueType>::BSTHandle() : nodePtr() { }

    template<Comparable KeyType, typename ValueType>
    BSTHandle<KeyType, ValueType>::BSTHandle(NodePtr _nodePtr) : nodePtr(std::move(_nodePtr)) { }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::get() {
        return this->nodePtr;
    }

    template<Comparable KeyType, typename ValueType>
    size_t BSTHandle<KeyType, ValueType>::size() const {
        return BSTHandle::size(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    size_t BSTHandle<KeyType, ValueType>::size(const NodePtr& nodePtr) {
        if (!nodePtr) {
            return 0;
        }

        return 1 + BSTHandle<KeyType, ValueType>::size(nodePtr->leftPtr) + BSTHandle<KeyType, ValueType>::size(nodePtr->rightPtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::insert(NodePtr &nodePtr, const KeyPtr &keyPtr, const ValuePtr &valuePtr) {
        if (!nodePtr) {
            nodePtr = makeEmptyNode<KeyType, ValueType>();
            nodePtr->keyPtr = keyPtr;
            nodePtr->valuePtr = valuePtr;
            return;
        }

        const KeyType& lhs = *keyPtr;
        const KeyType& rhs = *nodePtr->keyPtr;
        if (lhs > rhs) {
            BSTHandle<KeyType, ValueType>::insert(nodePtr->rightPtr, keyPtr, valuePtr);
        } else if (lhs < rhs) {
            BSTHandle<KeyType, ValueType>::insert(nodePtr->leftPtr, keyPtr, valuePtr);
        } else {
            nodePtr->keyPtr = keyPtr;
            nodePtr->valuePtr = valuePtr;
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::insert(const KeyPtr &keyPtr, const ValuePtr &valuePtr) {
        BSTHandle<KeyType, ValueType>::insert(this->nodePtr, keyPtr, valuePtr);
    }

    template<Comparable KeyType, typename ValueType>
    ValuePtr<ValueType> BSTHandle<KeyType, ValueType>::search(const NodePtr &nodePtr, const KeyPtr &keyPtr) {
        if (!nodePtr) {
            return nullptr;
        }

        const KeyType& lhs = *keyPtr;
        const KeyType& rhs = *nodePtr->keyPtr;
        if (lhs > rhs) {
            return BSTHandle<KeyType, ValueType>::search(nodePtr->rightPtr);
        } else if (lhs < rhs) {
            return BSTHandle<KeyType, ValueType>::search(nodePtr->leftPtr);
        } else {
            return nodePtr->valuePtr;
        }
    }

    template<Comparable KeyType, typename ValueType>
    ValuePtr<ValueType> BSTHandle<KeyType, ValueType>::search(const KeyPtr &keyPtr) {
        return BSTHandle<KeyType, ValueType>::search(this->nodePtr, keyPtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::min(const NodePtr &_nodePtr) {
        if (_nodePtr) {
            if (!_nodePtr->leftPtr) {
                return _nodePtr;
            }

            return BSTHandle<KeyType, ValueType>::min(_nodePtr->leftPtr);
        }

        return _nodePtr;
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::min() const {
        return BSTHandle<KeyType, ValueType>::min(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteMin(NodePtr &_nodePtr) {
        NodePtr head { _nodePtr };
        if (head) {
            if (head->leftPtr) {
                NodePtr prev { head };
                while (head->leftPtr) {
                    prev = head;
                    head = head->leftPtr;
                }

                prev->leftPtr = nullptr;
            } else {
                if (_nodePtr->rightPtr) {
                    Handle::assignNodePtr(_nodePtr, _nodePtr->rightPtr);
                } else {
                    _nodePtr->keyPtr = nullptr;
                    _nodePtr->valuePtr = nullptr;
                }
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    bool BSTHandle<KeyType, ValueType>::empty() const {
        return !this->nodePtr;
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteMin() {
        BSTHandle<KeyType, ValueType>::deleteMin(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::max(const NodePtr &_nodePtr) {
        if (_nodePtr) {
            if (!_nodePtr->rightPtr) {
                return _nodePtr;
            }

            return  BSTHandle<KeyType, ValueType>::max(_nodePtr->rightPtr);
        }

        return _nodePtr;
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteMax(NodePtr &_nodePtr) {
        NodePtr head { _nodePtr };
        if (head) {
            if (head->rightPtr) {
                NodePtr prev { head };
                while (head->rightPtr) {
                    prev = head;
                    head = head->rightPtr;
                }

                prev->rightPtr = nullptr;
            } else {
                if (_nodePtr->leftPtr) {
                    Handle::assignNodePtr(_nodePtr, _nodePtr->leftPtr);
                } else {
                    _nodePtr->keyPtr = nullptr;
                    _nodePtr->valuePtr = nullptr;
                }
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::max() const {
        return BSTHandle<KeyType, ValueType>::max(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteMax() {
        BSTHandle<KeyType, ValueType>::deleteMax(this->nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::reset(NodePtr _nodePtr) {
        this->nodePtr = std::move(_nodePtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::floor(const NodePtr &nodePtr, const KeyPtr &keyPtr) {
        return updateFloorCandidate(nodePtr, NodePtr {}, keyPtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::floor(const KeyPtr &keyPtr) const {
        return BSTHandle<KeyType, ValueType>::floor(this->nodePtr, keyPtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType>
    BSTHandle<KeyType, ValueType>::ceil(const NodePtr &currentNodePtr, const KeyPtr &keyPtr) {
        return updateCeilCandidate(currentNodePtr, NodePtr {}, keyPtr);
    }

    template<Comparable KeyType, typename ValueType>
    NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::ceil(const KeyPtr &keyPtr) const {
        return Handle::ceil(this->nodePtr, keyPtr);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::traverseInOrderLNR(
            const NodePtr &root,
            std::function<void(const NodePtr &)> fn,
            std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
            std::function<bool(const NodePtr &)> probePredicate
    ) {
        if (root && probePredicate(root)) {
            if (root->leftPtr && !prunePredicate(root, root->leftPtr)) {
                BSTHandle<KeyType, ValueType>::traverseInOrderLNR(
                        root->leftPtr,
                        fn,
                        prunePredicate,
                        probePredicate
                );
            }

            fn(root);

            if (root->rightPtr && !prunePredicate(root, root->rightPtr)) {
                BSTHandle<KeyType, ValueType>::traverseInOrderLNR(
                        root->rightPtr,
                        fn,
                        prunePredicate,
                        probePredicate
                );
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::traverseInOrderRNL(
            const NodePtr &root,
            std::function<void(const NodePtr &)> fn,
            std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
            std::function<bool(const NodePtr &)> probePredicate
    ) {
        if (root && probePredicate(root)) {
            if (root->rightPtr && !prunePredicate(root, root->rightPtr)) {
                BSTHandle<KeyType, ValueType>::traverseInOrderRNL(
                        root->rightPtr,
                        fn,
                        prunePredicate,
                        probePredicate
                );
            }

            fn(root);

            if (root->leftPtr && !prunePredicate(root, root->leftPtr)) {
                BSTHandle<KeyType, ValueType>::traverseInOrderRNL(
                        root->leftPtr,
                        fn,
                        prunePredicate,
                        probePredicate
                );
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::traverseInOrderLNR(std::function<void(const NodePtr &)> fn,
                                                           std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
                                                           std::function<bool(const NodePtr &)> probePredicate) {
        BSTHandle<KeyType, ValueType>::traverseInOrderLNR(this->nodePtr, fn, prunePredicate, probePredicate);
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::traverseInOrderRNL(std::function<void(const NodePtr &)> fn,
                                                           std::function<bool(const NodePtr& currentNodePtr, const NodePtr& enteringNodePtr)> prunePredicate,
                                                           std::function<bool(const NodePtr &)> probePredicate) {
        BSTHandle<KeyType, ValueType>::traverseInOrderRNL(this->nodePtr, fn, prunePredicate, probePredicate);
    }

    template<Comparable KeyType, typename ValueType>
    std::unique_ptr<std::vector<BST::NodePtr<KeyType, ValueType>>>
    BSTHandle<KeyType, ValueType>::rangeSearchMany(const NodePtr &root, const KeyType &lowerBound,
                                                   const KeyType &upperBound) {
        auto midPoint = BSTHandle<KeyType, ValueType>::rangeSearchOne(root, lowerBound, upperBound);
        auto resultVectorPtr = std::make_unique<std::vector<BST::NodePtr<KeyType, ValueType>>>();
        auto& result = *resultVectorPtr;
        BSTHandle<KeyType, ValueType>::traverseInOrderLNR(
                // 遍历起始点
                midPoint,

                // 节点操作函数
                [&result, &lowerBound, &upperBound](const auto &_nodePtr) mutable -> void {
                    const auto &key = *_nodePtr->keyPtr;
                    if (key < lowerBound || key > upperBound) {
                        // No op.
                    } else {
                        result.emplace_back(_nodePtr);
                    }
                },

                // 剪枝判别准则
                [&lowerBound, &upperBound](const auto &currentNodePtr, const auto &enteringNodePtr) -> bool {
                    const auto &key = *currentNodePtr->keyPtr;
                    if (key < lowerBound && enteringNodePtr == currentNodePtr->leftPtr) {
                        return true; // 若当前节点的 key 已经小于 lb, 那么放弃对当前节点的左子节点及其所有后代节点的探索。
                    }

                    if (key > upperBound && enteringNodePtr == currentNodePtr->rightPtr) {
                        return true; // 若当前节点的 key 已经大于 ub, 那么放弃对当前节点的右子节点及其所有后代节点的探索。
                    }

                    return false;
                },

                // 节点可进入性判定准则
                [](const auto &_) -> bool { return true; }
        );
        return resultVectorPtr;
    }

    template<Comparable KeyType, typename ValueType>
    std::unique_ptr<std::vector<BST::NodePtr<KeyType, ValueType>>>
    BSTHandle<KeyType, ValueType>::rangeSearchMany(const KeyType &lowerBound, const KeyType &upperBound) {
        return BSTHandle<KeyType, ValueType>::rangeSearchMany(this->nodePtr, lowerBound, upperBound);
    }

    template<Comparable KeyType, typename ValueType>
    BST::NodePtr<KeyType, ValueType> BSTHandle<KeyType, ValueType>::rangeSearchOne(
            const NodePtr &root,
            const KeyType &lowerBound,
            const KeyType &upperBound
    ) {
        if (root) {
            const KeyType& key = *root->keyPtr;
            if (key < lowerBound) {
                return rangeSearchOne(root->rightPtr, lowerBound, upperBound);
            } else if (key > upperBound) {
                return rangeSearchOne(root->leftPtr, lowerBound, upperBound);
            } else {
                return root;
            }
        }

        return root;
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteLeftChild(NodePtr &root) {
        if (root) {
            if (root->leftPtr) {
                BSTHandle<KeyType, ValueType>::deleteNodeInPlace(root->leftPtr);
                if (!(root->leftPtr->leftPtr || root->leftPtr->rightPtr)) {
                    root->leftPtr = nullptr;
                }
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteRightChild(NodePtr &root) {
        if (root) {
            if (root->rightPtr) {
                BSTHandle<KeyType, ValueType>::deleteNodeInPlace(root->rightPtr);
                if (!(root->rightPtr->leftPtr || root->rightPtr->rightPtr)) {
                    root->rightPtr = nullptr;
                }
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteNodeInPlace(NodePtr &childRef) {
        if (childRef) {
            childRef->keyPtr = nullptr;
            childRef->valuePtr = nullptr;

            if (childRef->leftPtr && childRef->rightPtr) {
                // 被删除的那个节点它既有左子节点又有右子节点
                // 现在的问题是 this->leftPtr 要怎么更新？
                // 设当前节点叫做 root, 被删除的那个节点（也就是 root 的左子节点）叫做 A, A 有左子节点 L, A 也有右子节点 R, 也就是如下图所示：
                //     root
                //     /
                //    /
                //   A
                //   | \
                //   |  \
                //   L   R
                // 思路是这样：被用来顶替 A 的位置的是 min(R), 这个 min(R) 其实就是 R 所代表的子树中的最小的节点，因为 R 是非空的，因此这个 min(R) 一定存在。

                NodePtr nodeA = childRef;
                NodePtr nodeR = nodeA->rightPtr;
                NodePtr minR = Handle::min(nodeR);
                NodePtr nodeL = nodeA->leftPtr;
                Handle::assignNodePtr(nodeA, minR);
                nodeA->leftPtr = nodeL;
                nodeA->rightPtr = nodeR;
                if (minR == nodeR) {
                    nodeA->rightPtr = nodeR->rightPtr;
                } else {
                    Handle::deleteMin(nodeR);
                }
            } else if (childRef->leftPtr) {
                // 被删除的那个节点它只有左子节
                Handle::assignNodePtr(childRef, childRef->leftPtr);
            } else if (childRef->rightPtr) {
                // 被删除的那个节点它只有右节点
                Handle::assignNodePtr(childRef, childRef->rightPtr);
            } else {
                // No op.
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::deleteKey(NodePtr &root, const KeyType &key) {
        const KeyType& rootKey = *root->keyPtr;
        if (rootKey == key) {
            Handle::deleteNodeInPlace(root);
            return;
        }

        std::deque<NodePtr> travelList { };
        travelList.emplace_back(root);
        while (!travelList.empty()) {
            NodePtr& head = travelList.front();
            travelList.pop_front();

            if (head->leftPtr) {
                if (*head->leftPtr->keyPtr == key) {
                    Handle::deleteLeftChild(head);
                    return;
                }

                travelList.emplace_back(head->leftPtr);
            }

            if (head->rightPtr) {
                if (*head->rightPtr->keyPtr == key) {
                    Handle::deleteRightChild(head);
                    return;
                }

                travelList.emplace_back(head->rightPtr);
            }
        }
    }

    template<Comparable KeyType, typename ValueType>
    void BSTHandle<KeyType, ValueType>::assignNodePtr(NodePtr &a, const NodePtr &b) {
        a->keyPtr = b->keyPtr;
        a->valuePtr = b->valuePtr;
        a->leftPtr = b->leftPtr;
        a->rightPtr = b->rightPtr;
    }
}

#endif //PRIORITYQUEUEIMPLEMENTATIONS_BINARYSEARCHTREE_HPP
