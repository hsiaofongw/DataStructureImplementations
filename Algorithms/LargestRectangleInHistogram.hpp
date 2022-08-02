//
// Created by 韦晓枫 on 2022/7/28.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_LARGESTRECTANGLEINHISTOGRAM_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_LARGESTRECTANGLEINHISTOGRAM_HPP

#include <vector>
#include <memory>
#include <functional>
#include <iostream>

namespace Algorithm::LargestRectangleInHistogram {

    using std::vector;

    class Solution {
    public:
        int largestRectangleArea(vector<int>& heights) {
            size_t N = heights.size();
            vector<size_t> leftGteCount (N, 0);
            vector<size_t> rightGteCount (N, 0);
            for (size_t i = 1; i < N; ++i) {
                if (heights[i] == 0) {
                    continue;
                }
                for (size_t dj = 1; dj <= i; ++dj) {
                    size_t j = i - dj;
                    if (heights[j] < heights[i]) {
                        break;
                    }
                    leftGteCount[i]++;
                }
            }

            for (size_t di = 1; di <= N-1; ++di) {
                size_t i = N-1-di;
                if (heights[i] == 0) {
                    continue;
                }
                for (size_t j = i+1; j < N; ++j) {
                    if (heights[j] < heights[i]) {
                        break;
                    }
                    rightGteCount[i]++;
                }
            }

            size_t maxArea = 0;
            for (size_t i = 0; i < N; ++i) {
                size_t h = heights[i];
                if (h != 0) {
                    size_t area = h + h * leftGteCount[i] + h * rightGteCount[i];
                    if (area > maxArea) {
                        maxArea = area;
                    }
                }
            }

            return static_cast<int>(maxArea);
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LARGESTRECTANGLEINHISTOGRAM_HPP
