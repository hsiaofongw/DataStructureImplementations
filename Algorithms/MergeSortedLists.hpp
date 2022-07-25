//
// Created by 韦晓枫 on 2022/7/25.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MERGESORTEDLISTS_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MERGESORTEDLISTS_HPP

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */

#include <vector>

namespace Algorithm::MergeSortedLists {



    struct ListNode {
        int val;
        ListNode *next;
    };

    /**
     * 合并 K 个已排好序的链表：
     *
     * 解法思路：
     * 1. 两层双指针，最外层那对双指针 i, j 指向 std::vector<ListNode*> lists 前后相邻两个元素（在这之前先判定 lists.size() >= 2);
     * 2. 内层双指针一个 ListNode *lhs 指向 lists[i] 的插入位置，一个指向 lists[j] 的当前扫描位置；
     * 3. 为了方便，可以临时建一个 ListNode dummy 对象，让 dummy.next 指向 lhs, 这样不管 lhs 是否为空，都可以方便地插入。
     */
    class Solution {
    public:
        ListNode* mergeKLists(std::vector<ListNode*>& lists) {

            size_t N = lists.size();

            if (N <= 1) {
                return flatArray(lists);
            }


            for (size_t di = 0; di < N-1; ++di) {
                size_t prevIdx = N-2-di;
                size_t currIdx = N-1-di;
                lists[prevIdx] = mergeTwoSortedLinkedList(lists[prevIdx], lists[currIdx]);
                lists.pop_back();
            }

            return flatArray(lists);
        }

    private:
        ListNode *mergeTwoSortedLinkedList(ListNode *prev, ListNode *curr) {
            ListNode dummy;
            ListNode *lhs = &dummy;
            lhs->next = prev;
            while (curr) {
                if (!(lhs->next)) {
                    lhs->next = curr;
                    return dummy.next;
                }

                if (lhs->next && curr->val <= lhs->next->val) {
                    auto temp = lhs->next;
                    lhs->next = curr;
                    curr = curr->next;
                    lhs->next->next = temp;
                    continue;
                }

                while (lhs && lhs->next && curr->val > lhs->next->val) {
                    lhs = lhs->next;
                }
            }

            return dummy.next;
        }

        ListNode *flatArray(const std::vector<ListNode*> &lists) {
            ListNode dummy;
            ListNode *dummyPtr = &dummy;
            for (const auto &lst : lists) {
                ListNode *head = lst;
                while (head) {
                    dummyPtr->next = head;
                    dummyPtr = dummyPtr->next;
                    head = head->next;
                }
            }

            return dummy.next;
        }
    };

}


#endif //DATASTRUCTUREIMPLEMENTATIONS_MERGESORTEDLISTS_HPP
