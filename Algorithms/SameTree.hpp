//
// Created by 韦晓枫 on 2022/8/14.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SAMETREE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SAMETREE_HPP

namespace Algorithm::SameTree {
    struct TreeNode {
        TreeNode ( ) : val(0), left(nullptr), right(nullptr) { }
        int val;
        TreeNode *left;
        TreeNode *right;
    };

    class Solution {
    public:
        bool isSameTree(TreeNode *lhs, TreeNode *rhs) {
            if (lhs && rhs) {
                return (lhs->val == rhs->val) && isSameTree(lhs->left, rhs->left) && isSameTree(lhs->right, rhs->right);
            } else if (lhs) {
                return false;
            } else if (rhs) {
                return false;
            } else {
                return true;
            }
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SAMETREE_HPP
