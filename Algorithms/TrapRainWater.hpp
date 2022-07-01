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

        class Solution {
        public:
            static int trap(const vector<int>& height) {

                // water(i) = max(min(maxHeight(0, i-1), maxHeight(i+1, N-1)) - height[i], 0);
                // DP solve and set down every maxHeight(0, i-1), every maxHeight(i+1, N-1)
                // sum up all water(i) for i = 1, 2, ..., N-2

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
    }
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_TRAPRAINWATER_HPP
