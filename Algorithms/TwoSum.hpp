//
// Created by 韦晓枫 on 2022/7/5.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_TWOSUM_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_TWOSUM_HPP

#include <utility>
#include <vector>
#include <cctype>
#include <algorithm>

namespace Algorithm {
    namespace TwoSum {
        /**
         * Two Sum 问题：
         *
         * 给定一个数组 nums: int[N], N >= 2, 以及一个整数 target: int,
         * 求下标对 (i, j) 使得 nums[i] + nums[j] == target,
         *
         * 约束：0 <= i < j <= N-1.
         *
         * 举例：nums = [2, 3, 5, 7, 9], target = 10,
         * 输出：(1, 3)，因为 nums[1] + nums[3] = 3 + 7 = 10 = target.
         *
         * 解法思路讨论：
         *
         * （一）穷举：
         *
         * 在搜索空间 { (i, j) : 0 <= i < j <= N-1 } 进行遍历，直到找到满足条件的。
         * 平均、最差时间复杂度：O(n), 空间复杂度 O(1).
         *
         * （二）备忘录解法：
         *
         * 声明一个 complementMap: std::map<int, size_t>,
         * 它将 target 与 nums 元素 num 的差与 nums 元素的下标关联起来。
         *
         * 举例来说，对于数组 nums = [2, 3, 5, 7], target = 10, 我们能得到这样的关联关系：
         * { 10-2 = 8 -> 0, 10-3 = 7 -> 1, 10-5 = 5 -> 2, 10-3 = 7 -> 3 }
         * 直观地理解为：假如当前还差 8 可以达成 target 10, 那么我们可以选 [0],
         * 假如当前还差 7 可以达成 target 10, 那么我们可以旋 [1],
         * 我们生成了这个 complementMap 之后，再遍历一遍 nums 数组，再遍历到第二个元素 (idx: 1) 时，3 和 10 还差 7,
         * 我们在 complementMap 里边查找，发现有 7 -> 3 这个关联关系，从而我们知道 nums[1] + nums[3] = target,
         * 输出 (1, 3).
         *
         * （三）二分搜索解法：
         *
         * 回顾一下，在（一）穷举法中，我们会去搜索 nums[i] 后边的 N-i-1 个元素来看是否有这样的，
         * 和 nums[i] 加起来之后恰好等于 target 的元素；在（二）备忘录解法中，
         * 我们只需花常数级别的时间复杂度，就可判定有没有另外一个元素和 nums[i] 加起来恰好等于 target,
         * 只不过，map 的插入操作通常来讲是对数级别的，所以法（二）的平均时间复杂度应该还是 O(n log n)
         *
         * 我们这回将排好序的数组本身看成索引，这是因为我看可以在一个排好序的数组上做二分搜索操作，
         * 二分搜索操作的平均时间复杂度是对数级别的，因此法（二）和法（三）差别不大。
         *
         * 下面我们实现一个二分搜索法：
         */

        class Solution {
        public:
            static std::pair<size_t, size_t> twoSum(
                const std::vector<int> &nums,
                int target
            ) {
                // 首先，因为 nums 是不可改变的，所以我们另起一个下标数组进行排序
                size_t N = nums.size();
                std::vector<size_t> indices (N);
                for (size_t i = 0; i < N; ++i)
                    indices[i] = i;

                std::sort(
                    indices.begin(),
                    indices.end(),
                    [&nums](size_t a, size_t b) { return nums[a] < nums[b]; }
                );

                // 后续我们就在 indices 上进行操作了，它是排好序了的

                // 先来计算差额
                std::vector<int> complements (N);
                for (size_t i = 0; i < N; ++i) {
                    complements[i] = target - nums[indices[i]];
                }
                // 由计算方法可知，complements 也是排好序的

                // 找 pair
                for (size_t i = 0; i < N; ++i) {
                    int num = nums[indices[i]];

                    // 这句对数级别时间复杂度的二分搜索操作代替了原先 map 查表操作
                    auto requireIt = std::lower_bound(
                        complements.begin(),
                        complements.end(),
                        num,
                        [](int a, int b) { return a > b; }
                    );

                    if (requireIt != complements.end()) {
                        if (*requireIt == num) {
                            size_t j = std::distance(complements.begin(), requireIt);
                            if (j != i) {
                                return std::pair<size_t, size_t> { indices[i], indices[j] };
                            }
                        }
                    }
                }

                assert((false));
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_TWOSUM_HPP
