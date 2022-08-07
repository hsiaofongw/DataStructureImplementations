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

        void renewKeyNode(NodePtr node) {
            // 首先找到这个 node 是在哪个池子
            NodePtr pool = pools[node->useCount];

            // 这个池子一定是非空的，因为至少有 node 这一个节点
            assert((!!pool));

            // 把 node 从池子取出
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

            // 把 node 放到下一个池子的顶部，
            // 首先，定位到下一个池子。
            node->useCount++;
            // 然后，分两种情况：
            if (pools[node->useCount]) {
                // 一种情况是：下一个池子是非空的，
                // 则让它作为这个池子的，新的首节点。
                NodePtr poolHead = pools[node->useCount];
                NodePtr poolTail = poolHead->pool.prev;
                node->pool.prev = poolTail;
                node->pool.next = poolHead;
                poolTail->pool.next = node;
                poolHead->pool.prev = node;
                pools[node->useCount] = node;

                // 更新 node 在 queue 中的位置
                NodePtr queueNext = node->pool.next;
                NodePtr queuePrev = queueNext->queue.prev;
                node->queue.prev = queuePrev;
                node->queue.next = queueNext;
                queueNext->queue.prev = node;
                queuePrev->queue.next = node;
            } else {
                // 另外一种情况：下一个池子是空的。
                // 这时直接赋值就行了。
                pools[node->useCount] = node;

                // 但是注意要让它自指成环，因为我们需要利用环的一些性质。
                node->pool.next = node;
                node->pool.prev = node;

                // 更新 node 在 queue 中的位置
                NodePtr oldPoolHead = pool;
                NodePtr prevNodeInQueue = oldPoolHead->queue.prev;
                node->queue.prev = prevNodeInQueue;
                node->queue.next = oldPoolHead;
                prevNodeInQueue->queue.next = node;
                oldPoolHead->queue.prev = node;
            }
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP
