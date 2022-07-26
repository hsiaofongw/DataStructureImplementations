//
// Created by 韦晓枫 on 2022/7/26.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_REORDERLIST_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_REORDERLIST_HPP

#include <deque>

namespace Algorithm::ReOrderList {
    struct ListNode {
        int val;
        ListNode *next;
    };

    class Solution {
    public:
        void reorderList(ListNode* head) {
            std::deque<ListNode*> q;

            while (head) {
                q.push_back(head);
                head = head->next;
            }

            if (q.size() <= 2) {
                return;
            }

            while (!q.empty()) {
                ListNode *l = q.front();
                q.pop_front();
                ListNode *r = q.back();
                q.pop_back();
                l->next = r;
                if (q.empty())
                    return;
                r->next = q.front();
                ListNode *back = q.back();
                back->next = nullptr;

                if (q.size() == 1) {
                    ListNode *front = q.front();
                    r->next = front;
                    front->next = nullptr;
                    return;
                }

            }
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_REORDERLIST_HPP
