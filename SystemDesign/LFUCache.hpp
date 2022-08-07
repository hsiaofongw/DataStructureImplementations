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

        explicit LFUCacheGen(int capacity) : tail(), addressMap(), capacity(capacity), pools() {

        }

        void put(KeyT key, ValT val) {
            auto keyNode = renewKey(key);
            if (keyNode)
                keyNode->val = val;
        }

        ValT get(KeyT key) {
            if (addressMap[key]) {
                auto keyNode = renewKey(key);
                return keyNode->val;
            }

            return defaultValue;
        }

        void printLink() {
            auto _head = tail->next;
            for (size_t i = 0; i < capacity; ++i) {
                std::cout << "(key=" << (_head->key) << ", " << "cnt=" << (_head->useCount) << ") ";
                _head = _head->next;
            }
            std::cout << std::endl;
        }

    private:

        size_t capacity;

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

        NodePtr tail;
        std::unordered_map<KeyT, NodePtr> addressMap;
        std::unordered_map<size_t, NodePtr> pools;

        /** 把 node 从池子取出 */
        void extractNodeFromPools(NodePtr node) {
            if (node->pool.prev == node) {
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
        }

        /** 更新 node 在队列中的位置 */
        void queueInsertAt(NodePtr node, NodePtr queuePrev, NodePtr queueNext) {
            (node->queue.prev)->queue.next = node->queue.next;
            (node->queue.next)->queue.prev = node->queue.prev;
            node->queue.next = queueNext;
            node->queue.prev = queuePrev;
            queuePrev->queue.next = node;
            queueNext->queue.prev = node;
        }

        /** 更新 node 在池子中的位置 */
        void poolInsertAt(NodePtr node, NodePtr poolPrev, NodePtr poolNext) {
            // 把 node 从池子里取出
            extractNodeFromPools(node);

            // 在新的位置建立链接
            node->pool.next = poolNext;
            node->pool.prev = poolPrev;
            poolPrev->pool.next = node;
            poolNext->pool.prev = node;
        }

        void moveForward(NodePtr node) {
            NodePtr tailRepl = tail->queue.prev;
            node->useCount++;

            NodePtr poolHead = pools[node->useCount];
            NodePtr queuePrev = poolHead->queue.prev;
            queueInsertAt(node, queuePrev, poolHead);

            NodePtr prevNode = node->queue.prev;
            if (node->useCount > prevNode->useCount) {
                if (pools[node->useCount]) {
                    poolHead = pools[node->useCount];
                } else {
                    poolHead = pools[(node->queue.prev)->useCount];
                }
                queueInsertAt(node, poolHead->queue.prev, poolHead);
            }

            if (node == tail) {
                tail = tailRepl;
            }
        }

        void poolInsertByIndex(NodePtr node, size_t poolIdx) {
            if (pools[poolIdx]) {
                NodePtr poolHead = pools[poolIdx];
                NodePtr poolTail = poolHead->pool.prev;
                poolInsertAt(node, poolTail, poolHead);
                pools[poolIdx] = node;
            } else {
                extractNodeFromPools(node);
                pools[poolIdx] = node;
                node->pool.prev = node;
                node->pool.next = node;
            }
        }

        void renewKeyNode(NodePtr node) {
            moveForward(node);
            poolInsertByIndex(node, node->useCount);
        }

        NodePtr assignNewKeyNode(const KeyT &key) {
            NodePtr node = tail;
            addressMap[node->key] = nullptr;
            addressMap[key] = node;
            node->key = key;
            node->useCount = 0;
            renewKeyNode(node);
            return node;
        }

        NodePtr renewKey(const KeyT &key) {
            if (addressMap[key]) {
                NodePtr node = addressMap[key];
                renewKeyNode(node);
                return node;
            }

            return assignNewKeyNode(key);
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP
