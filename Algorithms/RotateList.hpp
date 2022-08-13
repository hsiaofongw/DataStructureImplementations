//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_ROTATELIST_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_ROTATELIST_HPP

#include <vector>

namespace Algorithm::RotateLinkedList {

    struct ListNode {
        ListNode *next;
    };

    /**
     * 链表轮转：
     *
     * 关键点：
     *
     * 实际转动步数（指首尾连接成环后，从前往后，从 head 往 head->next, 再把 head 置为 head->next 的迭代的次数）：
     * 顺时针（正转）情况下是 k % N
     * 逆时针（反过来转）情况下是 N - k % N
     * N <= 1 则不用转。
     */
    class Solution {
    public:
        ListNode* rotateRight(ListNode* head, int k) {
            size_t N = 0;
            ListNode *root = head;
            ListNode *tail = nullptr;
            while (head) {
                ++N;
                tail = head;
                head = head->next;
            }

            if (N <= 1) {
                return root;
            }

            tail->next = root;

            int actualK = N - (k % N);
            // std::cout << "queueTail: " << (queueTail->val);
            // std::cout << "k: " << k << " actualK: " << actualK << "\n";
            // std::cout << "N: " << N << "\n";

            for (int i = 0; i < actualK; ++i) {
                tail = tail->next;
            }

            head = tail->next;
            tail->next = nullptr;

            return head;
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_ROTATELIST_HPP
