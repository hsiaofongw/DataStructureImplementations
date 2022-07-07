//
// Created by 韦晓枫 on 2022/7/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_DELETEELEMENTSAPPEARSMORETHANONCE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_DELETEELEMENTSAPPEARSMORETHANONCE_HPP

#include <cctype>

namespace Algorithm {
    namespace DeleteElementsAppearsMoreThanOnce {
        /**
         * 删除链表中出现次数超过 1 次（不含 1 次）的元素片段，
         * 例如：
         * 输入：1 -> 2 -> 2 -> 3 -> 5 -> 2 -> 9 -> 9 -> 9 -> nil
         *           x    x                   x    x    x
         *
         * 输出：1 -> 3 -> 5 -> 2 -> nil
         */

        struct ListNode {
            int val;
            ListNode *next;

            ListNode() : val(0), next(nullptr) {}

            explicit ListNode(int x) : val(x), next(nullptr) {}

            ListNode(int x, ListNode *next) : val(x), next(next) {}
        };

        class Solution {
        public:
            ListNode *deleteDuplicates(ListNode *head) {
                // 判空
                if (!head) {
                    return head;
                }

                // 求尾节点以及长度
                ListNode *start = head;
                ListNode *tail = nullptr;
                size_t len = 0;
                while (head) {
                    tail = head;
                    head = head->next;
                    ++len;
                }

                // 遍历，遍历的同时跳过重复值节点
                head = start;
                ListNode *tailWriter = tail;
                while (len > 0) {

                    int currentVal = head->val;
                    if ((head->next) && (currentVal == head->next->val)) {
                        while (len > 0 && (head->val == currentVal)) {
                            head = head->next;
                            --len;
                        }
                        continue;
                    }

                    // std::cout << "get: " << head->val << "\n";
                    tailWriter->next = head;
                    tailWriter = tailWriter->next;

                    head = head->next;
                    --len;
                }

                ListNode *result = tail->next;
                tailWriter->next = nullptr;
                tail->next = nullptr;

                return result;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_DELETEELEMENTSAPPEARSMORETHANONCE_HPP
