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

namespace SystemDesign::Cache {

    template <typename LFUCacheKeyT = int, typename LFUCacheValT = int, LFUCacheValT defaultValue = -1>
    class LFUCacheGen {
    public:

        explicit LFUCacheGen(int capacity) : tail(), addressMap(), capacity(capacity) {
            std::vector<std::shared_ptr<DoubleLinkedList<LFUDataT>>> temp;
            for (int i = 0; i < capacity; ++i) {
                auto node = std::make_shared<DoubleLinkedList<LFUDataT>>();
                node->data.keyUseCount = 0;
                temp.push_back(node);
            }

            for (int i = 0; i < capacity; ++i) {
                int prevIdx = (i+capacity-1)%capacity;
                int nextIdx = (i+1)%capacity;
                temp[i]->next = temp[nextIdx];
                temp[i]->prev = temp[prevIdx];
            }

            if (!temp.empty())
                this->tail = temp[(capacity-1)%capacity];
        }

        void put(LFUCacheKeyT key, LFUCacheValT val) {
            auto keyNode = renewKey(key);
            if (keyNode)
                keyNode->data.value = val;
        }

        LFUCacheValT get(LFUCacheKeyT key) {
            if (addressMap[key]) {
                auto keyNode = renewKey(key);
                return keyNode->data.value;
            }

            return defaultValue;
        }

        void printLink() {
            auto head = tail->next;
            for (size_t i = 0; i < capacity; ++i) {
                std::cout << "(key=" << (head->data.key) << ", " << "cnt=" << (head->data.keyUseCount) << ") ";
                head = head->next;
            }
            std::cout << std::endl;
        }

    private:

        size_t capacity;

        template <typename DataT>
        struct DoubleLinkedList {
            DoubleLinkedList() : prev(), next(), data() { }
            std::shared_ptr<DoubleLinkedList<DataT>> prev;
            std::shared_ptr<DoubleLinkedList<DataT>> next;
            DataT data;
        };

        struct LFUDataT {
            LFUCacheKeyT key;
            size_t keyUseCount;
            LFUCacheValT value;
        };

        std::shared_ptr<DoubleLinkedList<LFUDataT>> tail;
        std::unordered_map<LFUCacheKeyT, std::shared_ptr<DoubleLinkedList<LFUDataT>>> addressMap;

        std::shared_ptr<DoubleLinkedList<LFUDataT>> assignNewKey(LFUCacheKeyT key) {
            if (!tail)
                return tail;

            if (tail->data.keyUseCount == 0) {
                // 尾部还有空位
                tail->data.key = key;
                tail->data.keyUseCount = 1;
                addressMap[key] = tail;

                if (tail->prev->data.keyUseCount == 0) {
                    tail = tail->prev;
                    return tail->next;
                }

                auto node = tail;
                if (tail->prev->data.keyUseCount == 1) {
                    moveForward(tail);
                }

                return node;
            }

            addressMap[tail->data.key] = nullptr;
            addressMap[key] = tail;
            tail->data.key = key;
            tail->data.keyUseCount = 1;
            auto node = tail;
            if (tail->prev->data.keyUseCount == 1) {
                moveForward(tail);
            }

            return node;
        }

        bool moveForward(std::shared_ptr<DoubleLinkedList<LFUDataT>> node) {
            // 特例处理（被 renew 的那个 node 如果就是 head, 则无需（也无法）再继续前移）
            if (node == tail->next) {
                return false;
            }

            // 记下 prev 的位置，再把 node 移出
            auto prev = node->prev;
            prev->next = node->next;
            node->next->prev = prev;

            // 让 node 的两个触手分别抓住 prev 前面的，和 prev 后面的
            node->next = prev;
            node->prev = prev->prev;

            // 把现在的 node 的 prev, 以及现在的 node 的 next 都拉过来
            node->prev->next = node;
            node->next->prev = node;

            if (node == tail) {
                tail = tail->next;
            }

            return true;
        }

        std::shared_ptr<DoubleLinkedList<LFUDataT>> renewKeyNode(std::shared_ptr<DoubleLinkedList<LFUDataT>> node) {
            // 指针判空
            if (node) {
                // 自增使用计数
                ++(node->data.keyUseCount);
            } else {
                std::cerr << "尝试对一个空指针做 renew 操作。\n";
                assert(false);
            }

            // 自增使用计数之后，尝试前移指针。
            // 代码运行到这里可以保证 node 不是 head
            if ((node->data.keyUseCount) >= (node->prev->data.keyUseCount)) {
                moveForward(node);
            }

            return node;
        }

        std::shared_ptr<DoubleLinkedList<LFUDataT>> renewKey(LFUCacheKeyT key) {
            if (addressMap[key])
                return renewKeyNode(addressMap[key]);
            else
                return assignNewKey(key);
        }
    };

    class LFUCache {
    public:
        explicit LFUCache(int capacity) : cache(capacity) { }

        int get(int key) {
            return cache.get(key);
        }

        void put(int key, int value) {
            cache.put(key, value);
        }

        void printLink() {
            cache.printLink();
        }

        LFUCacheGen<> cache;
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP
