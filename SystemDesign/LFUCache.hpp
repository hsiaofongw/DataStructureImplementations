//
// Created by 韦晓枫 on 2022/7/30.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP

#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <deque>
#include <optional>

namespace SystemDesign::Cache {

    template <typename KeyT = int, typename ValT = int>
    class LFUCache {
    public:

        explicit LFUCache(size_t capacity) : queueTail(), addressMap(), pools() {
            queueHead = std::make_shared<Node>();
            queueTail = std::make_shared<Node>();
            queueHead->queue.next = queueTail;
            queueTail->queue.prev = queueHead;

            for (size_t i = 0; i < capacity; ++i) {
                NodePtr nodePtr = std::make_shared<Node>();
                queueInsertNodeBefore(nodePtr, queueTail);
            }

            NodePtr lastOne = queueTail->queue.prev;
            if (lastOne != queueHead) {
                // not empty, i.e. capacity != 0
                NodePtr firstOne = queueHead->queue.next;
                lastOne->pool.next = firstOne;
                firstOne->pool.prev = lastOne;
                for (NodePtr head = firstOne; head != lastOne; head=head->queue.next) {
                    NodePtr nextOne = head->queue.next;
                    head->pool.next = nextOne;
                    nextOne->pool.prev = head;
                }

                pools[0] = firstOne;
            }
        }

        bool put(KeyT key, ValT val) {
            if (auto keyNode = renewKey(key)) {
                keyNode->val = val;
                return true;
            }

            return false;
        }

        std::optional<ValT> get(KeyT key) {
            if (addressMap[key]) {
                auto keyNode = renewKey(key);
                return keyNode->val;
            }

            return {};
        }

        void printInternal(bool keyOnly = true) {
            printQueue(keyOnly);
            printPools(keyOnly);
        }

    private:
        struct Node;
        using NodePtr = std::shared_ptr<Node>;

        struct Link {
            NodePtr prev;
            NodePtr next;
        };

        struct Node {
            KeyT key;
            ValT val;
            size_t useCount;
            Link pool;
            Link queue;
        };

        NodePtr queueTail;
        NodePtr queueHead;
        std::unordered_map<KeyT, NodePtr> addressMap;
        std::unordered_map<size_t, NodePtr> pools;

        NodePtr deleteNodeInRotatedDoubleLinkedList(NodePtr head, NodePtr node) {
            if (head && node) {
                NodePtr nextHead = head;
                if (node == head) {
                    if (head->pool.prev == head) {
                        nextHead = nullptr;
                    } else {
                        nextHead = head->pool.next;
                    }
                }
                NodePtr prev = node->pool.prev;
                NodePtr next = node->pool.next;
                prev->pool.next = next;
                next->pool.prev = prev;

                node->pool.prev = node;
                node->pool.next = node;

                return nextHead;
            }
            assert((false));
        }

        void printKey(NodePtr node, bool keyOnly = true) {
            if (node) {
                if (keyOnly) {
                    std::cout << node->key << " ";
                } else {
                    std::cout << "(" << node->key << " -> " << node->val << ", " << "useCount: " << node->useCount << ") ";
                }
            }
        }

        void printPool(size_t poolIdx, bool keyOnly = true) {
            NodePtr poolHead = pools[poolIdx];
            if (poolHead) {
                NodePtr head = poolHead;
                std::cout << "[" << poolIdx << "]: ";
                while (true) {
                    printKey(head, keyOnly);
                    head = head->pool.next;
                    if (head == poolHead) {
                        break;
                    } else {
                        std::cout << "-> ";
                    }
                }
                std::cout << std::endl;
            }
        }

        void printQueue(bool keyOnly = true) {
            std::cout << "Queue: ";
            for (NodePtr head = queueHead->queue.next; head != queueTail; head = head->queue.next) {
                printKey(head, keyOnly);
                if (head->queue.next != queueTail) {
                    std::cout << "-> ";
                }
            }
            std::cout << std::endl;
        }

        void printPools(bool keyOnly = true) {
            std::cout << "Pools: " << std::endl;
            NodePtr prev = nullptr;
            NodePtr head = queueHead->queue.next;
            while (head != queueTail) {
                if ((!prev) || (prev->useCount != head->useCount)) {
                    printPool(head->useCount, keyOnly);
                }

                prev = head;
                head = head->queue.next;
            }
        }

        /** 把 node 从池子取出 */
        void detachFromPool(NodePtr node, size_t poolIdx) {
            NodePtr pool = pools[poolIdx];
            if (pool) {
                pools[poolIdx] = deleteNodeInRotatedDoubleLinkedList(pool, node);
                if (!pools[poolIdx]) {
                    pools.erase(poolIdx);
                }
            }
        }

        void queueInsertNodeBefore(NodePtr node, NodePtr position) {
            NodePtr queuePrev = position->queue.prev;
            node->queue.prev = queuePrev;
            node->queue.next = position;
            queuePrev->queue.next = node;
            position->queue.prev = node;
        }

        /** 把 node 指向的节点移动到 position 前面 */
        void queueMoveNodeBefore(NodePtr node, NodePtr position) {
            if (node != position) {
                detachFromQueue(node);
                queueInsertNodeBefore(node, position);
            }
        }

        void detachFromQueue(NodePtr node) {
            (node->queue.prev)->queue.next = node->queue.next;
            (node->queue.next)->queue.prev = node->queue.prev;
        }

        void  moveForward(NodePtr node) {
            size_t currentPoolIndex = node->useCount;
            size_t nextPoolIndex = currentPoolIndex+1;
            if (pools[nextPoolIndex]) {
                queueMoveNodeBefore(node, pools[nextPoolIndex]);
                return;
            }

            NodePtr head = pools[currentPoolIndex];
            if (head && (head != node)) {
                queueMoveNodeBefore(node, head);
            }
        }

        NodePtr poolInsertBefore(NodePtr node, NodePtr head) {
            if (head) {
                NodePtr nextHead = node;
                NodePtr prev = head->pool.prev;
                node->pool.prev = prev;
                prev->pool.next = node;
                node->pool.next = head;
                head->pool.prev = node;
                return nextHead;
            } else {
                return node;
            }
        }

        void poolInsertByIndex(NodePtr node, size_t poolIdx) {
            detachFromPool(node, node->useCount);
            pools[poolIdx] = poolInsertBefore(node, pools[poolIdx]);
        }

        void renewKeyNode(NodePtr node) {
            moveForward(node);
            poolInsertByIndex(node, node->useCount+1);
            node->useCount++;
        }

        NodePtr assignNewKeyNode(const KeyT &key) {
            NodePtr node = queueTail->queue.prev;

            // retire
            if (addressMap[node->key]) {
                addressMap[node->key] = nullptr;
                addressMap.erase(node->key);
            }
            detachFromPool(node, node->useCount);
            node->useCount = 0;

            // re-hire
            addressMap[key] = node;
            node->key = key;
            renewKeyNode(node);

            return node;
        }

        NodePtr renewKey(const KeyT &key) {
            if (queueHead->queue.next == queueTail) {
                return nullptr;
            }

            NodePtr node = addressMap[key];
            if (node) {
                renewKeyNode(node);
                return node;
            }

            return assignNewKeyNode(key);
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP
