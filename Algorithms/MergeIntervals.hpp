//
// Created by 韦晓枫 on 2022/7/17.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MERGEINTERVALS_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MERGEINTERVALS_HPP

#include <vector>
#include <algorithm>
#include <cstdint>

namespace Algorithm {
    namespace MergeIntervals {
        /**
         * 合并部分重叠的区间：
         *
         * 问题描述：
         * 给你一个关于区间的数组 vector<vector<int>> intervals,
         * 如果其中有任意两个区间是部分重叠的，你就要把这两个区间合并起来。
         *
         * 举例：
         * 输入：intervals = [[1,3],[2,6],[8,10]]
         * 输出：output = [[1,6],[8,10]]
         * 解释：[1,3] 和 [2,6] 有重叠，所以要把它两合并，
         * [8,10] 和其它任何区间都没有重叠，所以不用被合并。
         *
         * 解法思路：
         * 先对 intervals 按照区间的左端点升序排序，确保只要 i <= j 就有 intervals[i][0] <= intervals[j][0],
         * 然后用双指针技术：右指针指向的区间和左区间有重叠，就让右指针指向的区间并入左指针指向的区间来，
         * 如果右指针指向的区间和左指针没有重叠，就更新左指针为右指针，然后右指针右移一步，如此循环，
         * 直至所有重叠区间都已被合并，并且右指针跳出数组范围为止。
         *
         * 补充说明：
         * 这里所有区间看作是闭区间，因此对于区间 D1=[a,b] 和 D2=[c,d]，当 b=c 时 D1 和 D2 也被认为是有重叠的。
         */

        using std::vector;

        std::vector<std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>>> getTestCases() {
            return {
                    {{{1,3},{2,6},{8,10},{15,18}}, {{1,6},{8,10},{15,18}}},
                    {{{1,4},{4,5}},{{1,5}}}
            };
        }

        class Solution {
        public:
            vector<vector<int>> merge(vector<vector<int>>& intervals) {
                std::sort(
                        intervals.begin(),
                        intervals.end(),
                        [](const vector<int> &a, const vector<int> &b) -> bool {
                            return a[0] < b[0];
                        }
                );

                intptr_t leftPtr = 0;
                std::vector<intptr_t> mergedIntervalPtrList { leftPtr };
                for (intptr_t rightPtr = 1; rightPtr < intervals.size(); ++rightPtr) {
                    if (intervals[rightPtr][0] > intervals[leftPtr][1]) {
                        // 不重叠
                        leftPtr = rightPtr;
                        mergedIntervalPtrList.push_back(leftPtr);
                    } else {
                        // 重叠
                        // 那么就将 rightPtr 指向的区间并入 leftPtr 指向的区间，
                        // 具体怎么做就是更新 leftPtr 指向的区间的上确界
                        intervals[leftPtr][1] = std::max(intervals[leftPtr][1], intervals[rightPtr][1]);
                    }
                }

                std::vector<std::vector<int>> mergedIntervals;
                for (const auto &ptr : mergedIntervalPtrList) {
                    mergedIntervals.push_back(intervals[ptr]);
                }

                return mergedIntervals;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MERGEINTERVALS_HPP
