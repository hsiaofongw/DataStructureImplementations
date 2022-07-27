//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_RECOVERBINARYSEARCHTREE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_RECOVERBINARYSEARCHTREE_HPP

#include <vector>

namespace Algorithm::RecoverBinarySearchTree {
    struct TreeNode {
        int val;
        TreeNode *left;
        TreeNode *right;
        TreeNode() : val(0), left(nullptr), right(nullptr) {}
        explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
        TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
    };

    struct TestCase {
        TreeNode *root;
        TreeNode *expectedOutput;
    };

    std::vector<TestCase> getTestCases() {
        std::vector<TestCase> testCases;


        return testCases;
    }

    /**
     * 注意：你不要直接调用这个函数！
     *
     * 你应该只负责调用 getTestCases 函数和 freeTestCases 函数，
     * 并且 getTestCases 函数和 freeTestCases 函数应该配对！
     */
    void freeTree(TreeNode *root) {
        if (root) {
            freeTree(root->left);
            freeTree(root->right);
            delete root;
        }
    }

    void freeTestCases(std::vector<TestCase> &testCases) {
        for (auto &testCase : testCases) {
            freeTree(testCase.root);
            freeTree(testCase.expectedOutput);
        }
    }

    class Solution {
    public:
        void recoverTree(TreeNode *root) {
            std::vector<TreeNode*> trace;
            traverseInorder(root, trace);
            size_t N = trace.size();
            if (N >= 2) {
                std::vector<int> nums;
                for (auto &node : trace)
                    nums.push_back(node->val);
                std::sort(nums.begin(), nums.end());
                for (size_t i = 0; i < nums.size(); ++i)
                    if (trace[i]->val != nums[i])
                        trace[i]->val = nums[i];
            }
        }

    private:
        /** 中序遍历一颗二叉树 */
        void traverseInorder(TreeNode *root, std::vector<TreeNode*> &trace) {
            if (root) {
                traverseInorder(root->left, trace);
                trace.push_back(root);
                traverseInorder(root->right, trace);
            }
        }

        /** 交换值 */
        void exchangeValue(TreeNode *a, TreeNode *b) {
            if (a) {
                if (b) {
                    auto temp = a->val;
                    a->val = b->val;
                    b->val = temp;
                }
            }
        }
    };
}



#endif //DATASTRUCTUREIMPLEMENTATIONS_RECOVERBINARYSEARCHTREE_HPP
