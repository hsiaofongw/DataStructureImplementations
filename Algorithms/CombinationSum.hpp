//
// Created by 韦晓枫 on 2022/8/1.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_COMBINATIONSUM_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_COMBINATIONSUM_HPP

#include <vector>

namespace Algorithm::CombinationSum {

    /**
     * 和为特定值的所有组合问题：
     *
     * 给定一非空非负数组 vector<int> candidates, 以及一个非负的 int target，
     * 试找出 candidates 中所有不重复的和为 target 的组合。
     *
     * 补充说明：
     * 1. 数组 candidates 中没有重复元素；
     * 2. 一个组合是一个非空的 vector<int>, 一个组合列表是一个 vector<vector<int>>, 组合列表可以是空的；
     * 3. 两个组合 vector<int> a, vector<int> b 重复是指：当分别对 a, b 升序排序后，它们每个对应位置的值都相等；
     *
     * 思路：
     * 这题我们可以采用分治法：
     * 1. 先找出所有没有 candidates[0] 的组合，
     * 2. 再找出所有有 candidates[0] 的组合，
     * 3. 最后再把这两个组合列表合并起来返回给上一层。
     */

    using std::vector;

    class Solution {
    public:
        vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
            size_t N = candidates.size();
            assert((N >= 1));
            std::sort(candidates.begin(), candidates.end());
            std::vector<size_t> indexCounter;
            indexCounter.resize(N, 0);
            std::vector<decltype(indexCounter)> indexCounters;
            getCombinations(0, N, candidates, target, indexCounter, indexCounters);
            vector<vector<int>> combinations;
            for (const auto &counter : indexCounters) {
                vector<int> combination;
                for (size_t i = 0; i < N; ++i) {
                    size_t count = counter[i];
                    for (size_t j = 0; j < count; ++j) {
                        combination.emplace_back(candidates[i]);
                    }
                }
                combinations.emplace_back(combination);
            }
            return combinations;
        }
    private:
        void getCombinations(
            size_t i,
            size_t N,
            vector<int> &candidates,
            int target,
            vector<size_t> &indexCounter,
            vector<vector<size_t>> &indexCounters
        ) {
            if (i < N && candidates[i] <= target) {
                // there are still elements to pick up

                // try to ignore the first element
                getCombinations(i+1, N, candidates, target, indexCounter, indexCounters);

                // pick up the first element
                target -= candidates[i];

                // record this "pick up" action on the indexCounter
                indexCounter[i]++;

                // and get combinations of the rest
                getCombinations(i, N, candidates, target, indexCounter, indexCounters);

                // restore the indexCounter when return, since it is shared by all the invocations of this function.
                indexCounter[i]--;
            } else {
                // no elements to pick up
                if (target == 0)
                    indexCounters.emplace_back(indexCounter);

                return;
            }
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_COMBINATIONSUM_HPP
