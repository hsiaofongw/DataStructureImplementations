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

        explicit LFUCacheGen(int capacity) : tail(), addressMap(), capacity(capacity), useCountMap() {
            std::vector<std::shared_ptr<ListNode>> temp;
            for (int i = 0; i < capacity; ++i) {
                auto node = std::make_shared<ListNode>();
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

        struct ListNode {
            ListNode() : prev(), next(), useCount(), key(), val() { }
            std::shared_ptr<ListNode> prev;
            std::shared_ptr<ListNode> next;
            size_t useCount;
            KeyT key;
            ValT val;
        };

        using NodePtr = std::shared_ptr<ListNode>;

        NodePtr tail;
        std::unordered_map<KeyT, NodePtr> addressMap;
        std::unordered_map<size_t, std::deque<NodePtr>> useCountMap;

        void popNodeFromCurrentQueue(NodePtr node) {
            std::deque<NodePtr> &q = useCountMap[node->useCount];
            assert((!q.empty()));
            q.pop_back();
            if (q.empty()) {
                useCountMap.erase(node->useCount);
            }
        }

        void insertNodeIntoNextQueue(NodePtr &node) {
            node->useCount++;
            std::deque<NodePtr> &q = useCountMap[node->useCount];
            q.push_front(node);

            if (node->prev == node) {
                return;
            }

            if (q.size() >= 2) {
                NodePtr mostRecent = q[1];
                detach(node);
                insertBefore(mostRecent, node);
            }
        }

        NodePtr assignNewKey(const KeyT &key) {
            popNodeFromCurrentQueue(tail);

            tail->useCount = 0;
            addressMap[tail->key] = nullptr;
            tail->key = key;
            addressMap[key] = tail;

            insertNodeIntoNextQueue(tail);

            tail = tail->prev;
            return tail->next;
        }

        void renewKeyNode(NodePtr node) {
            popNodeFromCurrentQueue(node);
            insertNodeIntoNextQueue(node);
        }

        void detach(NodePtr &node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        void insertBefore(NodePtr &pos, NodePtr &node) {
            node->prev = pos->prev;
            node->next = pos;
            node->prev->next = node;
            node->next->prev = node;
        }

        NodePtr renewKey(const KeyT &key) {
            if (addressMap[key]) {
                NodePtr node = addressMap[key];
                renewKeyNode(node);
                return node;
            }

            return assignNewKey(key);
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LFUCACHE_HPP
