//
// Created by 韦晓枫 on 2022/7/17.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MINPATHSUM_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MINPATHSUM_HPP

#include <vector>
#include <utility>
#include "../Interfaces/Matrix.hpp"

namespace Algorithm {
    namespace MinPathSum {

        using std::vector;
        using std::pair;

        vector<pair<Interface::IntMatrix, int>> getTestCases() {
            return {
                    {{{1,3,1},{1,5,1},{4,2,1}}, 7},
                    {{{1,2,3},{4,5,6}}, 12}
            };
        }

        class Solution {
        public:
            int minPathSum(vector<vector<int>>& grid) {
                // 公式：
                // 设 N, M 分别为行数、列数，且 N, M >= 1, grid 是 N*M 矩阵
                // f[N-1][M-1] = grid[N-1][M-1]
                // f[i][j] = grid[i][j] + min(f[i+1][j], f[i][j+1]), 其中 i != N-1 且 j != M-1.

                size_t nRows = grid.size();
                size_t nCols = grid[0].size();
                vector<vector<int>> minPathSumValue (nRows);
                for (auto &row : minPathSumValue) {
                    row.resize(nCols);
                }

                // 算 f[N-1][M-1]
                minPathSumValue[nRows-1][nCols-1] = grid[nRows-1][nCols-1];

                // 算 f[N-1][...]
                for (size_t jOffset = 1; jOffset < nCols; ++jOffset) {
                    size_t j = nCols-1-jOffset;
                    minPathSumValue[nRows-1][j] = grid[nRows-1][j] + minPathSumValue[nRows-1][j+1];
                }

                // 算 f[...][M-1]
                for (size_t iOffset = 1; iOffset < nRows; ++iOffset) {
                    size_t i = nRows-1-iOffset;
                    minPathSumValue[i][nCols-1] = grid[i][nCols-1] + minPathSumValue[i+1][nCols-1];
                }

                // 算剩余的部分
                // 从 f[N-2][M-2] 开始，顺序是这样：
                // f[N-2][M-2], f[N-2][M-3], ..., f[N-2][0],
                // ...
                // f[0][M-2], f[0][M-3], ..., f[0][0]
                for (size_t iOffset = 1; iOffset < nRows; ++iOffset) {
                    for (size_t jOffset = 1; jOffset < nCols; ++jOffset) {
                        size_t i = nRows-1-iOffset;
                        size_t j = nCols-1-jOffset;
                        minPathSumValue[i][j] = grid[i][j] + std::min(
                            minPathSumValue[i+1][j],
                            minPathSumValue[i][j+1]
                        );
                    }
                }

                // 得出答案
                return minPathSumValue[0][0];
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MINPATHSUM_HPP
