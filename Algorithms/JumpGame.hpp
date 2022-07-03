//
// Created by 韦晓枫 on 2022/7/4.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_JUMPGAME_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_JUMPGAME_HPP

#include <cctype>
#include <vector>

namespace Algorithm {
    namespace JumpGame {
        using std::vector;

        /**
         * Jump Game 问题：
         * 设有一个非空数组 uint strength[N], N >= 1, 对于 0 <= i <= N-1,
         * strength[i] 表示当你处于位置 i 时，最大可前移的步长，
         *
         * 例如，假设 strength = [2, 3, 1, 1, 4],
         * 那么，由于 strength[1] = 3, 因此当你处于位置 1 时，最多可前移 3 步，到达位置 4.
         *
         * 假设你的初始位置是 0, 你需要根据得到的 strength: uint[N] 数组来判断，你能否到达位置 N-1.
         *
         * 思路：
         * 定义函数 f: 下标 -> { true, false } 表示任意非越界下标 i 的可达性，
         * 则依题意有：f(0) = true,
         * 并且，对于 1 <= i <= N-1 的下标，f(i) 为真当且仅当下列 i 个表达式中的任何一个表达式为真：
         * 表达式 f(0) 且 f(0) >= i,
         * 表达式 f(1) 且 f(1) + 1 >= i,
         * 表达式 f(2) 且 f(2) + 2 >= i,
         * ...
         * 表达式 f(i-1) 且 f(i-1) + i-1 >= i.
         *
         * 进一步分析可知，若已知 f(0), f(1), ..., f(i-1) 皆可达，
         * 则 f(i) 是否可达完全取决于 max(f(0), f(1)+1, f(2)+2, ..., f(i-1)+i-1) >= i 是否为真。
         *
         * 由上述讨论得知，我们完全可以在从左往右遍历数组的过程中不断扩张当前的最大可达范围，
         * 直到这个最大可达范围覆盖住 N-1 这个位置。
         *
         * 具体实现见下列代码：
         */

        class Solution {
        public:
            static bool canJump(vector<int>& nums) {
                size_t maxOffset = 0; // 已知最大可达 [0]
                size_t targetOffset = nums.size()-1; // maxOffset 增长目标
                for (size_t offset = 0; offset <= maxOffset; ++offset) {
                    // 尝试扩张 maxOffset, 直到 maxOffset 等于甚至超过目标
                    maxOffset = std::max(maxOffset, nums[offset]+offset);

                    if (maxOffset >= targetOffset) {
                        return true;
                    }
                }

                return false;
            }
        };
    }
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_JUMPGAME_HPP
