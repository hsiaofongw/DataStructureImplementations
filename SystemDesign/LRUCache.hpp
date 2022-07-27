//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_LRUCACHE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_LRUCACHE_HPP

#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

namespace SystemDesign::LRUCache {

    template <typename LRUCacheKeyT = int, typename LRUCacheValT = int, LRUCacheValT defaultValue = -1>
    class LRUCacheGen {
    public:

        explicit LRUCacheGen(int capacity) : tail(), addressMap() {
            assert((capacity >= 1));
            std::vector<std::shared_ptr<DoubleLinkedList<LRUDataT>>> temp;
            for (int i = 0; i < capacity; ++i) {
                auto node = std::make_shared<DoubleLinkedList<LRUDataT>>();
                temp.push_back(node);
            }

            for (int i = 0; i < capacity; ++i) {
                int prevIdx = (i+capacity-1)%capacity;
                int nextIdx = (i+1)%capacity;
                temp[i]->next = temp[nextIdx];
                temp[i]->prev = temp[prevIdx];
            }

            this->tail = temp[(capacity-1)%capacity];
        }

        void put(LRUCacheKeyT key, LRUCacheValT val) {
            auto keyNode = renewKey(key);
            keyNode->data.value = val;
        }

        LRUCacheValT get(LRUCacheKeyT key) {
            if (addressMap[key]) {
                auto keyNode = renewKey(key);
                return keyNode->data.value;
            }

            return defaultValue;
        }

    private:

        template <typename DataT>
        struct DoubleLinkedList {
            DoubleLinkedList() : prev(), next(), data() { }
            std::shared_ptr<DoubleLinkedList<DataT>> prev;
            std::shared_ptr<DoubleLinkedList<DataT>> next;
            DataT data;
        };

        struct LRUDataT {
            LRUCacheKeyT key;
            LRUCacheValT value;
        };

        std::shared_ptr<DoubleLinkedList<LRUDataT>> tail;
        std::unordered_map<LRUCacheKeyT, std::shared_ptr<DoubleLinkedList<LRUDataT>>> addressMap;

        std::shared_ptr<DoubleLinkedList<LRUDataT>> assignNewKey(LRUCacheKeyT key) {
            addressMap[tail->data.key] = nullptr;
            addressMap[key] = tail;
            tail->data.key = key;
            auto keyNode = tail;
            tail = tail->prev;
            return keyNode;
        }

        std::shared_ptr<DoubleLinkedList<LRUDataT>> renewKeyNode(std::shared_ptr<DoubleLinkedList<LRUDataT>> node) {
            if (node == tail) {
                tail = tail->prev;
                return tail->next;
            }

            node->prev->next = node->next;
            node->next->prev = node->prev;

            node->next = tail->next;
            node->next->prev = node;
            node->prev = tail;
            node->prev->next = node;

            return node;
        }

        std::shared_ptr<DoubleLinkedList<LRUDataT>> renewKey(LRUCacheKeyT key) {
            if (addressMap[key])
                return renewKeyNode(addressMap[key]);
            else
                return assignNewKey(key);
        }
    };

    class LRUCache {
    public:
        explicit LRUCache(int capacity) : cache(capacity) { }

        int get(int key) {
            return cache.get(key);
        }

        void put(int key, int value) {
            cache.put(key, value);
        }

    private:
        LRUCacheGen<int, int, -1> cache;
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LRUCACHE_HPP
