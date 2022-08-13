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

namespace SystemDesign::Cache {

    template <typename KeyT = int, typename ValT = int, ValT defaultValue = -1>
    class LFUCacheGen {
    public:

        explicit LFUCacheGen(int capacity) : queueTail(), addressMap(), pools() {
            queueHead = std::make_shared<ListNode>();
            queueTail = std::make_shared<ListNode>();
            queueHead->queue.next = queueTail;
            queueTail->queue.prev = queueHead;

            for (size_t i = 0; i < capacity; ++i) {
                NodePtr node = std::make_shared<ListNode>();
                queueInsertNodeBefore(node, queueTail);
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

        void put(KeyT key, ValT val) {
            if (auto keyNode = renewKey(key))
                keyNode->val = val;
        }

        ValT get(KeyT key) {
            if (addressMap[key]) {
                auto keyNode = renewKey(key);
                return keyNode->val;
            }

            return defaultValue;
        }

        void printInternal() {
            printQueue();
            printPools();
        }

    private:
        struct ListNode;
        using NodePtr = std::shared_ptr<ListNode>;

        struct Link {
            NodePtr prev;
            NodePtr next;
        };

        struct ListNode {
            ListNode() : pool(), queue(), useCount(), key(), val() { }
            Link pool;
            Link queue;
            size_t useCount;
            KeyT key;
            ValT val;
        };

        NodePtr queueTail;
        NodePtr queueHead;
        std::unordered_map<KeyT, NodePtr> addressMap;
        std::unordered_map<size_t, NodePtr> pools;

        void printKey(NodePtr node) {
            if (node) {
                std::cout << "(" << node->key << "," << node->useCount << ") ";
            }
        }

        void printPool(NodePtr poolHead, size_t poolIdx) {
            if (poolHead) {
                NodePtr head = poolHead;
                std::cout << "pool " << poolIdx << ": ";
                while (true) {
                    printKey(head);
                    head = head->pool.next;
                    if (head == poolHead) {
                        break;
                    }
                }
                std::cout << std::endl;
            }
        }

        void printQueue() {
            std::cout << "queue: ";
            for (NodePtr head = queueHead->queue.next; head != queueTail; head = head->queue.next) {
                printKey(head);
            }
            std::cout << std::endl;
        }

        void printPools() {
            for (auto &pair : pools) {
                const size_t poolIdx = pair.first;
                NodePtr poolHead = pair.second;
                printPool(poolHead, poolIdx);
            }
        }

        /** 把 node 从池子取出 */
        void detachFromPool(NodePtr node) {
            if (node->pool.prev == node && pools[node->useCount] == node) {
                // 池子中只有 node 这一个
                pools[node->useCount] = nullptr;

                // 现在池子空了，pools 是一个字典，也最好把对应的键值对释放掉，否则，
                // 长此以往 unordered_map 的性能会因为有大量废弃不用的键值对而下降。
                pools.erase(node->useCount);
            } else {
                // 池子中有多个 node, 则让 node 的上一个和下一个直接相连，
                // 这就相当于把 node 从池子中取出了。
                NodePtr prev = node->pool.prev;
                NodePtr next = node->pool.next;
                prev->pool.next = next;
                next->pool.prev = prev;
            }

            // 让 node 自指是必要的，因为当它被放到另外一个 pool 之后，
            // 我们不希望它还继续和当前 pool 的节点保持有联系（通过 .pool.next 或者 .pool.prev）。
            node->pool.prev = node;
            node->pool.next = node;
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
            assert((node != position));
            detachFromQueue(node);
            queueInsertNodeBefore(node, position);
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

        void poolInsertBefore(NodePtr node, NodePtr position) {
            assert((node != position));
            NodePtr prev = position->pool.prev;
            node->pool.prev = prev;
            node->pool.next = position;
            prev->pool.next = node;
            position->pool.prev = node;
        }

        void poolInsertByIndex(NodePtr node, size_t poolIdx) {
            detachFromPool(node);
            if (pools[poolIdx]) {
                poolInsertBefore(node, pools[poolIdx]);
            }
            pools[poolIdx] = node;
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
            detachFromPool(node);
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
