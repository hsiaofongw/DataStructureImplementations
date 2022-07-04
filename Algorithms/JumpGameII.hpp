//
// Created by 韦晓枫 on 2022/7/5.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_JUMPGAMEII_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_JUMPGAMEII_HPP

#include <vector>
#include <cctype>

namespace Algorithm {
    namespace JumpGameII {
        using std::vector;

        /**
         * Jump Game 问题：
         * 设有一个非空数组 uint strength[N], N >= 1, 对于 0 <= i <= N-1,
         * strength[i] 表示当你处于位置 i 时，最大可前移的步长，
         *
         * 例如，假设 strength = [2, 3, 1, 1, 4],
         * 那么，由于 strength[1] = 3, 因此当你处于位置 1 时，最多可前移 3 步，到达位置 4.
         *
         * 假设输入的数组 strength: int[N] 总是能保证位置 N-1 是可达的，
         * 试求从位置 0 到达位置 N-1 所需的 *最小步数*.
         *
         * 思路：
         *
         * 我们采用贪心算法来求解该问题。
         *
         * 假设我们当前是在 i_k 位置，记 m = strength[i_k],
         * 则 [i_k + 1, i_k + m] 这个闭区间代表我们在 i_k 上的这一跳能到达的位置的范围，
         * 换句话说, 对任意一个满足 i_k + 1 <= i <= i_k + m 的 i, i_k 到 i 都只需 1 步，
         * 当然，i_k 到 i_k 需要 0 步。
         *
         * 然后我们在 [i_k + 1, i_k + m] 这 m 个位置中寻找一个使得 strength[i]+i 最大的 i, 记它为 i_{k+1},
         * 那么，i_{k+1} + strength[i_{k+1}] 是我们从 i_k 开始再走 2 步能触及的最大范围的右确界
         * 也就是说，再走 2 步，最远能走到 i_{k+1} + strength[i_{k+1}] 这个位置。
         *
         * 现在，总结一下，对于 k = 0,1,2,3,..., 我们能够算出走 k 步最远能走到多远，
         * 记 f[k] 表示从下标位置 0 开始 走 k 步能到达的最远位置，
         * 则我们完全可以用迭代式或者递归式的方法找到这样的最小的 k_{min} 满足 f[k_{min}] >= strength.size()-1.
         *
         * 具体实现见下列代码：
         */

        class Solution {
        public:
            int jump(vector<int>& nums) {
                if (nums.size() == 1) {
                    return 0;
                }

                return doJump(nums, 0, 1);
            }

        private:
            int doJump(vector<int>& nums, size_t beginIdx, int steps) {
                size_t lastIdx = nums.size() - 1;
                size_t strength = static_cast<size_t>(nums[beginIdx]);
                if (beginIdx + strength >= lastIdx) {
                    return steps;
                }

                size_t maxIdx = std::min(lastIdx, beginIdx + strength);
                size_t nextStart = beginIdx + 1;
                size_t maxRange = nextStart + nums[nextStart];
                for (size_t idx = nextStart; idx <= maxIdx; ++idx) {
                    if (idx + static_cast<size_t>(nums[idx]) > maxRange) {
                        nextStart = idx;
                        maxRange = nextStart + nums[nextStart];
                    }
                }

                return doJump(nums, nextStart, steps+1);
            }
        };
    }
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_JUMPGAMEII_HPP
