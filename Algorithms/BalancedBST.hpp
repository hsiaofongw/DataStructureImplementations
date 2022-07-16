//
// Created by 韦晓枫 on 2022/7/16.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_BALANCEDBST_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_BALANCEDBST_HPP

#include <cstdint>

namespace Algorithm {
    namespace BalancedBST {

        /**
         * 平衡二叉搜索树的定义：树的每一个节点的左右子树的高度差都不超过 1.
         *
         * 思路：递归计算每个节点的高度的过程中顺便判断一个即可，
         * 如果超过了就通过引用置 isBalanced = false.
         */

        struct TreeNode {
            TreeNode *left;
            TreeNode *right;
        };

        class Solution {
        public:
            bool isBalanced(TreeNode* root) {
                bool isBalanced = true;
                checkHeightBalance(root, isBalanced);
                return isBalanced;
            }

        private:
            uint32_t checkHeightBalance(TreeNode *root, bool &isBalanced) {
                if (root) {
                    uint32_t height = 1;
                    uint32_t leftHeight = checkHeightBalance(root->left, isBalanced);
                    uint32_t rightHeight = checkHeightBalance(root->right, isBalanced);

                    // 必要时交换，使得 leftHeight 总是较大的那个
                    if (leftHeight < rightHeight) {
                        uint32_t tmp = leftHeight;
                        leftHeight = rightHeight;
                        rightHeight = tmp;
                    }

                    uint32_t heightDiff = leftHeight - rightHeight;
                    if (heightDiff > 1) {
                        isBalanced = false;
                    }

                    return height + leftHeight;
                }

                return 0;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_BALANCEDBST_HPP
