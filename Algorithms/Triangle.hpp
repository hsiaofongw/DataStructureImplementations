//
// Created by 韦晓枫 on 2022/7/16.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_TRIANGLE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_TRIANGLE_HPP

#include <vector>

namespace Algorithm {
    namespace Triangle {
        /**
         * Triangle 问题：
         *
         * 问题描述：
         *
         * vector<vector<int>> triangle 是一个 N 行的矩阵，N >= 1,
         * triangle[i] 的长度为 i+1（有 i+1 个元素）。
         *
         * 一条路径 vector<size_t> link 满足这样的条件：
         * link.size() == triangle.size(), 且 link[i+1] 只能为 link[i] 或者 link[i]+1, link[0] = 0.
         *
         * 一条路径的长度是 int distance = triangle[link[0]] + ... + triangle[link[triangle.size()-1]].
         *
         * 求最短的这样的路径的长度。
         *
         * 思路：
         *
         * 从 triangle 的第 i 行第 j 列出发，上述这样的最短路径的长度记做 f[i][j],
         * 注意到：f[i][j] = triangle[i][j] + min(f[i+1][j], f[i+1][j+1]).
         *
         * 例如：f[0][0] = triangle[0][0] + min(f[1][0], f[1][1])
         *              = triangle[0][0] + min(
         *                                     triangle[1][0] + min(f[2][0], f[2][1]),
         *                                     triangle[1][1] + min(f[2][1], f[2][2])
         *                                 )
         *
         *  我们注意到，如果直接展开这个式子， f[2][1] 将会被重复计算，后面还会有更多的 f[i][j] 被重复计算。
         *
         *  所以我们可以使用动态规划的思想，自底向上地构筑这样的 f[i][j] 的值，
         *  记录在一个长度与 triangle 的高度相当的数组中，
         *  并且在自底向上遍历这个 triangle 的过程中，一层一层地更新这个数组。
         */

        using std::vector;

        class Solution {
        public:
            int minimumTotal(vector<vector<int>>& triangle) {
                size_t height = triangle.size();
                std::vector<int> minDistances;
                for (const auto &d : triangle[height-1]) {
                    minDistances.push_back(d);
                }

                for (size_t rowOffset = 1; rowOffset < height; ++rowOffset) {
                    size_t rowIdx = height-1-rowOffset;
                    const vector<int> &row = triangle[rowIdx];
                    const size_t rowSize = height-rowOffset;

                    for (size_t colIdx = 0; colIdx < rowSize; ++colIdx) {
                        minDistances[colIdx] = row[colIdx] + std::min(
                                minDistances[colIdx],
                                minDistances[colIdx+1]
                        );
                    }

                    // for (const auto &x : minDistances) {
                    //    std::cout << x << " ";
                    // }
                    // std::cout << "\n";
                }

                return minDistances[0];
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_TRIANGLE_HPP
