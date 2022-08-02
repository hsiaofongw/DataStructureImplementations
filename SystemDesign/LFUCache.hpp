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

    template <typename LFUCacheKeyT = int, typename LFUCacheValT = int, LFUCacheValT defaultValue = -1>
    class LFUCacheGen {
    public:

        explicit LFUCacheGen(int capacity) : tail(), addressMap(), capacity(capacity), useCountMap() {
            std::vector<std::shared_ptr<DoubleLinkedList>> temp;
            for (int i = 0; i < capacity; ++i) {
                auto node = std::make_shared<DoubleLinkedList>();
                node->useCount = 0;
                useCountMap[0].push_back(node);
                temp.push_back(node);
            }

            for (int i = 1; i < capacity; ++i) {
                int prevIdx = i-1;
                temp[i]->prev = temp[prevIdx];
                temp[i]->prev->next = temp[i];
            }

            if (!temp.empty()) {
                temp[0]->prev = temp[capacity-1];
                tail = temp[0]->prev;
                tail->next = temp[0];
            }
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
            auto _head = tail->next;
            for (size_t i = 0; i < capacity; ++i) {
                std::cout << "(key=" << (_head->data.key) << ", " << "cnt=" << (_head->data.keyUseCount) << ") ";
                _head = _head->next;
            }
            std::cout << std::endl;
        }

    private:

        size_t capacity;

        struct DoubleLinkedList {
            DoubleLinkedList() : prev(), next(), useCount(), key(), val() { }
            std::shared_ptr<DoubleLinkedList> prev;
            std::shared_ptr<DoubleLinkedList> next;
            size_t useCount;
            LFUCacheKeyT key;
            LFUCacheValT val;
        };

        std::shared_ptr<DoubleLinkedList> tail;
        std::unordered_map<LFUCacheKeyT, std::shared_ptr<DoubleLinkedList>> addressMap;
        std::unordered_map<size_t, std::deque<DoubleLinkedList>> useCountMap;

        std::shared_ptr<DoubleLinkedList> assignNewKey(LFUCacheKeyT key) {
            if (useCountMap[0].empty()) {
                // 没有 free 空间
                // 那就释放一个
                addressMap[tail->key] = nullptr;
                tail->useCount = 0;
                auto newTail = tail->prev;
                detach(tail);
                useCountMap[0].push_back(tail);
                tail = newTail;
            }

            assert((!useCountMap[0].empty()));

            std::shared_ptr<DoubleLinkedList> node = useCountMap[0].front();
            useCountMap[0].pop_front();
            node->key = key;
            node->useCount = 1;
            addressMap[key] = node;


            return node;
        }

        std::shared_ptr<DoubleLinkedList> renewKeyNode(std::shared_ptr<DoubleLinkedList> node) {

        }

        void detach(std::shared_ptr<DoubleLinkedList> node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        std::shared_ptr<DoubleLinkedList> renewKey(LFUCacheKeyT key) {
            if (addressMap[key])
                return renewKeyNode(addressMap[key]);
            else
                return assignNewKey(key);
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP
