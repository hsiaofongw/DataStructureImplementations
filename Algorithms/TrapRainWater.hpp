//
// Created by 韦晓枫 on 2022/7/1.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_TRAPRAINWATER_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_TRAPRAINWATER_HPP

#include <vector>
#include <algorithm>

namespace Algorithm {

    namespace TrapRainWater {
        using std::vector;

        vector<vector<int>> getTestCases() {
            vector<vector<int>> testCases {
                {0,1,0,2,1,0,1,3,2,1,2,1},
                {5,4,1,2}
            };
            return testCases;
        };

        class Solution1 {
        public:
            static int trap(const vector<int>& height) {

                // water(i) = max(min(maxHeight(0, i-1), maxHeight(i+1, N-1)) - height[i], 0);
                // DP solve and set down every maxHeight(0, i-1), every maxHeight(i+1, N-1)
                // sum up all water(i) for i = 1, 2, ..., N-2

                // 思路是这样的：
                //
                // 下雨会使得坑洼处积水，积水的高度会先增长，水溢出时会往低处流淌，最终水的高度会稳定，
                // 这时我们说这个积水的过程收敛了，系统到达了一个稳定的状态。
                //
                // 不难验证，在稳定时，对于非越界下标 i, 且 1 <= i <= N-2, i 处的积水量为：
                //
                // water(i) = max(
                //   0,
                //   min( max(height[0..i]), max(height[i+1..N]) ) - height[i]
                // )
                //
                // 最外层套上 max 是为了保证这个值非负，
                // min( max(height[0..i]), max(height[i+1..N]) ) 决定了 i 处的水位，
                // min( max(height[0..i]), max(height[i+1..N]) ) 与 height[i] 一起共同决定了 i 处的实际储水量。
                //
                // 注意到 0 处和 N-1 处的最终储水量都为 0, 因此最终求雨水量之和时无需考虑这两个地方。
                //
                // 并且注意到，max(height[0..i]) 当 i 取遍 0, 1, 2, ..., N-1 时是可借助于动态规划 (Dynamic Programming) 的方法求值的，
                // 同样，对于 max(height[i+1..N]) 来说也是。下列的代码中，我们用 leftMaxHeights 向量存储 max(height[0..i]) 的值,
                // 用 rightMaxHeights 存储 max(height[i+1..N]) 的值，并且总共只需遍历两遍 height 向量就可以算出这两个数组。
                //
                // 在那之后，直接套公式求 water(i) 的值并且求和，就可以得到最终结果了。

                if (std::empty(height)) {
                    return 0;
                }

                vector<int> leftMaxHeights (height.size());
                vector<int> rightMaxHeights (height.size());
                size_t s = height.size();
                leftMaxHeights[0] = height[0];
                rightMaxHeights[s-1] = height[s-1];

                {

                    size_t maxIdx = 0;
                    for (size_t idx = 0; idx < height.size(); ++idx) {
                        if (height[idx] > height[maxIdx]) {
                            maxIdx = idx;
                        }
                        leftMaxHeights[idx] = height[maxIdx];
                    }

                    maxIdx = s-1;
                    for (size_t offset = 0; offset < height.size(); ++offset) {
                        size_t idx = height.size() - 1 - offset;
                        if (height[idx] > height[maxIdx]) {
                            maxIdx = idx;
                        }
                        rightMaxHeights[idx] = height[maxIdx];
                    }
                }

                int rains = 0;
                for (size_t idx = 1; idx < s-1; ++idx) {
                    rains += std::max(0, std::min(leftMaxHeights[idx-1], rightMaxHeights[idx+1]) - height[idx]);
                }

                return rains;
            }
        };

        class Solution2 {
        public:
            static int trap(const vector<int>& height) {

                // 在 Solution1 中，我们用动态规划的技巧计算出了每一个 max(height[0..i])  以及每一个 max(height[i+1..N]),
                // 并且将这些值存储在向量中，这样做，会导致程序需要 O(n) 的额外空间。
                //
                // 现在，我们希望挑战一下：把这 O(n) 的额外空间也去掉，让程序只用 O(1) 的额外空间就可以完成计算。
                //
                // 接下来我们要作出的改进是基于这样的事实：
                //
                // 设 i_m 是向量 height 的最大值的下标，并且如果 height 有多个最大值，那么 i_m 是这些最大值的下标中最小的那一个。
                // 那么，对于 i_m 以及所有 i_m 左边的下标 0 <= i <= i_m, max(height[i..N]) == height[i_m] 恒成立。
                // 并且，对于 i_m 以及所有 i_m 右边的下标 i_m <= i <= N-1, max(height[0..i+1]) == height[i_m] 也是恒成立。
                //
                // 我们在从最左边遍历到 i_m 的过程中就能够算出每一个 max(height[0..i]), 同样在从最右边遍历到 i_m 的过程中，
                // 也能够计算出每一个 max(height[i..N]), 如果在这之前我们就算出了 i_m,
                // 那么我们就不需要任何空间来存储这些 max(height[0..i]) 以及 max(height[i..N]) 的值了。

                size_t s = height.size();
                int rains = 0;
                if (std::empty(height) || s <= 2) {
                    return rains;
                }

                // 求上面说的那个 i_m
                size_t maxIdx = 0;
                for (size_t idx = 0; idx < s; ++idx) {
                    if (height[idx] > height[maxIdx]) {
                        maxIdx = idx;
                    }
                }

                // 从左往右遍历
                int leftMax = height[0];
                for (size_t idx = 1; idx <= maxIdx; ++idx) {
                    rains += std::max(0, std::min(leftMax, height[maxIdx]) - height[idx]);
                    if (height[idx] > leftMax) {
                        leftMax = height[idx];
                    }
                }

                // 从右往左遍历
                int rightMax = height[s-1];
                for (size_t offset = 0, idx; s-1-offset > maxIdx; ++offset) {
                    idx = s-1-offset;
                    rains += std::max(0, std::min(height[maxIdx], rightMax) - height[idx]);
                    if (height[idx] > rightMax) {
                        rightMax = height[idx];
                    }
                }

                return rains;
            }
        };
    }
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_TRAPRAINWATER_HPP
