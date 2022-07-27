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
    class LRUCache {
    public:

        explicit LRUCache(int capacity) : linkedListHandle(), addressMap() {
            auto tail = DoubleLinkedListManipulator<LRUCacheKeyT>::constructList(static_cast<size_t>(capacity));
            this->linkedListHandle.setTail(tail);
        }

        void put(LRUCacheKeyT key, LRUCacheValT val) {
            auto keyNode = renewKey(key);
            storage[keyNode->data] = val;
        }

        LRUCacheValT get(LRUCacheKeyT key) {
            if (addressMap[key]) {
                auto keyNode = renewKey(key);
                return storage[keyNode->data];
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

        template <typename KeyT>
        class DoubleLinkedListManipulator {
        public:
            static std::shared_ptr<DoubleLinkedList<KeyT>> constructList(size_t size) {
                assert((size >= 1));

                auto head = std::make_shared<DoubleLinkedList<KeyT>>();
                head->next = head;
                head->prev = head;

                DoubleLinkedListManipulator<KeyT> handle;
                auto tail = head->prev;
                handle.setTail(tail);
                for (size_t i = 1; i < size; ++i) {
                    auto x = std::make_shared<DoubleLinkedList<KeyT>>();
                    handle.insertAfter(tail, x);
                }

                return tail;
            }

            void setTail(std::shared_ptr<DoubleLinkedList<KeyT>> x) {
                this->tail = x;
            }

            std::shared_ptr<DoubleLinkedList<KeyT>> getTail() {
                return tail;
            }

            void rotateCounterClockwise() {
                tail = tail->prev;
            }

            void insertAfter(std::shared_ptr<DoubleLinkedList<KeyT>> position, std::shared_ptr<DoubleLinkedList<KeyT>> x) {
                x->next = position->next;
                x->next->prev = x;
                position->next = x;
                position->next->prev = position;
            }

            void insertAfterTail(std::shared_ptr<DoubleLinkedList<KeyT>> x) {
                insertAfter(tail, x);
            }

            void detach(std::shared_ptr<DoubleLinkedList<KeyT>> x) {
                x->prev->next = x->next;
                x->next->prev = x->prev;
            }
        private:
            std::shared_ptr<DoubleLinkedList<KeyT>> tail;
        };

        DoubleLinkedListManipulator<LRUCacheKeyT> linkedListHandle;
        std::unordered_map<LRUCacheKeyT, std::shared_ptr<DoubleLinkedList<LRUCacheKeyT>>> addressMap;
        std::vector<LRUCacheValT> storage;

        void revokeOldest() {
            auto tail = linkedListHandle.getTail();
            auto oldKey = tail->data;
            addressMap[oldKey] = nullptr;
        }

        std::shared_ptr<DoubleLinkedList<LRUCacheKeyT>> assignNewKey(LRUCacheKeyT key) {
            revokeOldest();
            auto tail = linkedListHandle.getTail();
            addressMap[key] = tail;
            tail->data = key;
            linkedListHandle.rotateCounterClockwise();
            return tail->next;
        }

        std::shared_ptr<DoubleLinkedList<LRUCacheKeyT>> renewKeyNode(std::shared_ptr<DoubleLinkedList<LRUCacheKeyT>> node) {
            linkedListHandle.detach(node);
            linkedListHandle.insertAfterTail(node);
            return node;
        }

        std::shared_ptr<DoubleLinkedList<LRUCacheKeyT>> renewKey(LRUCacheKeyT key) {
            if (addressMap[key])
                return renewKeyNode(addressMap[key]);
            else
                return assignNewKey(key);
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LRUCACHE_HPP
