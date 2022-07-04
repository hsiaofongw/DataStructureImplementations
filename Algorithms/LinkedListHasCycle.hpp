//
// Created by 韦晓枫 on 2022/7/5.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_LINKEDLISTHASCYCLE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_LINKEDLISTHASCYCLE_HPP


namespace Algorithm {
    namespace LinkedListHasCycle {

        /**
         * 链表有环判定问题：
         * 设 ListNode* head 可能是一个空指针（用 nullptr 表示），也可能指向一个链表的首节点，
         * 对于非空链表 ListNode* head, 若链表中有任一节点 ListNode* x 满足下列情形中的任何一种，
         * 则我们说 head 指向一个有环的链表：
         *
         * (1) 表达式 x->next == x 为真；
         * (2) 存在 x 的直接或者间接后代节点 y, 使得表达式 y->next == x 为真；
         *
         * 试实现一个函数 bool hasCycle(ListNode* head) 判定 head 所指的链表是否有环。
         *
         * 思路：
         *
         * 若 head 是一个空指针，则环不存在。
         *
         * 若 head 不是空指针，说明它指向一个非空链表，非空链表的首节点一定存在，
         * 则我们可以分别初始化一个快指针 ListNode *fast, 一个慢指针 ListNode *slow 都指向 head 指向的节点。
         *
         * 慢指针每次走一步，即将 slow 本身的值更新为其直接子节点指针的值，再将 fast 更新为 fast->next->next (如果 fast->next 非 nullptr）。
         *
         * 若 head 指向的链表是无环的，则 slow 或者 fast 在更新的过程中会被置 nullptr.
         *
         * 若 head 指向的链表是有环的，则 slow 或者 fast 可以永远这样更新下去，
         * 但是因为 fast 比 slow 快，fast 早晚会从后边重新追上 slow, 这时表达式 fast == slow 求值得 true.
         *
         * 具体实现见下列代码：
         */

        struct ListNode {
          int val;
          ListNode *next;
          ListNode(int x) : val(x), next(nullptr) {}
        };

        class Solution {
        public:
            bool hasCycle(ListNode *head) {
                ListNode *slow = head;
                ListNode *fast = head;

                while (slow != nullptr && fast != nullptr) {
                    slow = slow->next;

                    fast = fast->next;
                    if (fast == nullptr) {
                        break;
                    }
                    fast = fast->next;

                    if (slow == fast) {
                        if (slow == nullptr) {
                            break;
                        }
                        else {
                            return true;
                        }
                    }
                }

                return false;
            }
        };
    }
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_LINKEDLISTHASCYCLE_HPP
