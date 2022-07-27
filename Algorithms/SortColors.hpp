//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SORTCOLORS_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SORTCOLORS_HPP

#include <vector>

namespace Algorithm::SortColors {
    /**
     * 排序颜色：
     *
     * 问题描述：
     *
     * 用 0, 1, 2 代表三种颜色，一个数组 std::vector<int> a 中只会出现 0, 1, 2 这三种值，
     * 你要做的就是对 a 排序，使得 0 和 0 挨着，然后是 1 和 1 挨着，最后是 2 和 2 挨着，举个例子：
     * Input = [ 0, 1, 1, 0, 1, 2, 0, 2 ]
     * Output = [0, 0, 0, 1, 1, 1, 2, 2 ]
     *
     * 解题思路：数数
     */
    class Solution {
    public:
        void sortColors(std::vector<int>& nums) {
            uint32_t colorCount[3];
            colorCount[0] = 0;
            colorCount[1] = 0;
            colorCount[2] = 0;
            for (const int &num : nums) {
                colorCount[num] += 1;
            }

            for (size_t i = 0; i < colorCount[0]; ++i)
                nums[i] = 0;
            for (size_t i = 0; i < colorCount[1]; ++i)
                nums[i+colorCount[0]] = 1;
            for (size_t i = 0; i < colorCount[2]; ++i)
                nums[i+colorCount[1]+colorCount[0]] = 2;
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SORTCOLORS_HPP
