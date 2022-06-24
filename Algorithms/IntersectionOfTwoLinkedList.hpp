//
// Created by 韦晓枫 on 2022/6/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_INTERSECTIONOFTWOLINKEDLIST_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_INTERSECTIONOFTWOLINKEDLIST_HPP

#include <cctype>

namespace Algorithm {
    namespace IntersectionOfTwoLinkedList {

        struct ListNode {
            int val;
            ListNode *next;

            explicit ListNode(int x) : val(x), next(nullptr) {}
        };

        class Solution {
        public:
            static ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
                size_t lengthA = 0;
                auto head = headA;
                while (head) {
                    head = head->next;
                    ++lengthA;
                }

                if (lengthA == 0) {
                    return nullptr;
                }

                size_t lengthB = 0;
                head = headB;
                while (head) {
                    head = head->next;
                    ++lengthB;
                }

                if (lengthB == 0) {
                    return nullptr;
                }

                ListNode *shortPtr = nullptr;
                ListNode *longPtr = nullptr;
                size_t maxLen = 0;
                size_t minLen = 0;
                if (lengthA > lengthB) {
                    longPtr = headA;
                    shortPtr = headB;
                    maxLen = lengthA;
                    minLen = lengthB;
                } else {
                    longPtr = headB;
                    shortPtr = headA;
                    maxLen = lengthB;
                    minLen = lengthA;
                }

                for (size_t i = 0; i < maxLen - minLen; ++i) {
                    longPtr = longPtr->next;
                }

                // 现在，longPtr 和 shortPtr 一样长

                // 同步前进

                while (longPtr && shortPtr) {
                    if (longPtr == shortPtr) {
                        return longPtr;
                    }

                    longPtr = longPtr->next;
                    shortPtr = shortPtr->next;
                }

                return nullptr;
            }
        };
    }
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_INTERSECTIONOFTWOLINKEDLIST_HPP
