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
            if (heights.empty())
                return 0;

            size_t N = heights.size();
            vector<size_t> distinctIndices (N);
            vector<size_t> repeats (N);
            size_t uniqueCount;
            compress(heights, distinctIndices, repeats, uniqueCount);
            vector<int> distinct;
            for (const size_t &idx : distinctIndices)
                distinct.push_back(heights[idx]);

            vector<size_t> leftAreas (uniqueCount);
            vector<size_t> rightAreas (uniqueCount);
            leftAreas[0] = 0;
            rightAreas[uniqueCount-1] = 0;

            for (size_t i = 1; i < uniqueCount; ++i) {
                if (distinct[i] == 0) {
                    continue;
                }
                for (size_t dj = 1; dj <= i; ++dj) {
                    size_t j = i - dj;
                    if (distinct[j] < distinct[i]) {
                        break;
                    }
                    leftAreas[i] += distinct[i] * repeats[j];
                }
            }

            for (size_t di = 1; di+1 <= uniqueCount; ++di) {
                size_t i = uniqueCount-1-di;
                if (distinct[i] == 0) {
                    continue;
                }
                for (size_t j = i+1; j < uniqueCount; ++j) {
                    if (distinct[j] < distinct[i]) {
                        break;
                    }
                    rightAreas[i] += distinct[i] * repeats[j];
                }
            }

            size_t maxArea = 0;
            for (size_t i = 0; i < uniqueCount; ++i) {
                size_t h = distinct[i];
                if (h != 0) {
                    size_t area = h * repeats[i] + leftAreas[i] + rightAreas[i];
                    if (area > maxArea) {
                        maxArea = area;
                    }
                }
            }

            return static_cast<int>(maxArea);
        }
    private:
        template <typename ElementT>
        void compress(
            const vector<ElementT> &origin,
            vector<size_t> &distinctIndices,
            vector<size_t> &repeats,
            size_t &uniqueCnt
        ) {
            if (origin.empty()) {
                distinctIndices.resize(0);
                repeats.resize(0);
                return;
            }

            const size_t N = origin.size();
            distinctIndices[0] = 0;
            repeats[0] = 1;
            uniqueCnt = 1;
            for (size_t i = 0, j = 1; j < N; ++j) {
                if (origin[i] == origin[j]) {
                    repeats[uniqueCnt-1]++;
                } else {
                    repeats[uniqueCnt] = 1;
                    distinctIndices[uniqueCnt] = j;
                    ++uniqueCnt;
                    i = j;
                }
            }

            distinctIndices.resize(uniqueCnt);
            repeats.resize(uniqueCnt);
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LARGESTRECTANGLEINHISTOGRAM_HPP
