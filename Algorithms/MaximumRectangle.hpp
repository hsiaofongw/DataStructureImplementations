//
// Created by 韦晓枫 on 2022/7/13.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMRECTANGLE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMRECTANGLE_HPP

#include <vector>
#include <algorithm>
#include "../Interfaces/Matrix.hpp"
#include <unordered_map>
#include <cstdio>
#include <queue>

namespace Algorithm {
    namespace MaximumRectangle {

        using std::vector;
        using Matrix = Interface::Matrix<char>;
        using TestCase = std::pair<Matrix, int>;
        using TestCases = std::vector<TestCase>;

        template <typename T>
        using HashMap = std::unordered_map<size_t, T>;

        /**
         * 最大矩形：
         *
         * 问题描述：给定规格为 N * M 的矩阵 vector<vector<int>> matrix, 其中 1 <= N, M,
         * 试找出 matrix 中最大的这样的元素全为 1 的子区域 [i, i+h) * [j, j+w)，并返回其面积。
         *
         * 解题思路：
         *
         * 设函数 f: (i, j, w, h) -> bool 定义为：
         * f(i, j, w, h) = {
         *   true, 若矩形域 [i, i+h) * [j, j+w) 中的元素全都为 1;
         *   false, otherwise.
         * }
         *
         * 则原问题转变为：
         * 找到最大的这样的 w_max, h_max, 使得存在非越界的 i, j 使得 f(i, j, w_max, h_max) == true.
         *
         * 我们尝试用动态规划技巧来计算 f 对于各个有效输入的值。
         *
         * 首先，将 f 表为两个更小子问题的答案的运算：
         *
         * 式 (1): f(i, j, w, h) = f(i, j, w, 1) && f(i+1, j, w, h-1), 其中：要求 h >= 2, 0 <= i+h <= N, 0 <= j, j+w <= M;
         * 式 (2): f(i, j, w, 1) = f(i, j, w-1, 1) && f(i, j+w-1, 1, 1), 其中：要求 w >= 2, 0 <= i < N, 0 <= j, j+w <= M;   式 (2)
         * 式 (3): f(i, j, 1, 1) = matrix[i][j] == 1, 其中：要求 0 <= i < N, 0 <= j < M.
         *
         * 这就完整定义了 f.
         *
         * f 的定义也揭示了如何用 DP 的方法去一步一步构筑它对于更大规模输入值的答案。
         */

        TestCases getTestCases() {
            return {
                    {{{'1','1','1','1','1','1','1','1'},{'1','1','1','1','1','1','1','0'},{'1','1','1','1','1','1','1','0'},{'1','1','1','1','1','0','0','0'},{'0','1','1','1','1','0','0','0'}}, 21},
                    {{{'1','0','1','0','0'},{'1','0','1','1','1'},{'1','1','1','1','1'},{'1','0','0','1','0'}}, 6},
                    {{{'0'}}, 0},
                    {{{'1'}}, 1},
                    {{{'1','0'},{'1','0'}}, 2}
            };
        }

        class Solution {

        public:
            int maximalRectangle(vector<vector<char>>& matrix) {
                auto nRows = matrix.size();
                auto nCols = matrix[0].size();
                using SizeT = decltype(nRows);
                auto extendRect = [&nRows, &nCols, &matrix](SizeT i, SizeT j) -> SizeT {
                    SizeT maxRectArea = 0;
                    for (SizeT w = 1; j + w <= nCols && matrix[i][j+w-1] == '1'; ++w) {
                        bool isRect = true;
                        for (SizeT h = 1; i + h <= nRows && isRect; ++h) {
                            for (SizeT dJ = 0; dJ < w && isRect; ++dJ) {
                                isRect = matrix[i+h-1][j+dJ] == '1';
                            }
                            if (isRect) {
                                maxRectArea = std::max(maxRectArea, w * h);
                            }
                        }
                    }

                    return maxRectArea;
                };

                SizeT maxRectArea = 0;
                for (SizeT i = 0; i < nRows; ++i) {
                    for (SizeT j = 0; j < nCols; ++j) {
                        maxRectArea = std::max(maxRectArea, extendRect(i, j));
                    }
                }

                return static_cast<int>(maxRectArea);
            }
        };
    }
}



#endif //DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMRECTANGLE_HPP
