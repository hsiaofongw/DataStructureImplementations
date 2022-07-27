//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMSQUARE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMSQUARE_HPP

#include <vector>

namespace Algorithm::MaximumSquare {
    /**
     * 最大正方形问题
     *
     * 问题描述：
     *
     * 给定一个矩阵 std::vector<std::vector<int>> matrix, 是找到最大的
     * 只包含 1 的正方形子区域并返回其面积。
     *
     * 补充说明：
     *
     * 一个矩形子区域是一个坐标集合，它也可以由这样的两个闭区间集合的笛卡尔积集合表示：
     *
     * [i0, i0+h) * [j0, j0+w)
     *
     * 也可以直接用关于下标的集合来表示：
     *
     * 例如，子区域 [1, 1+2) * [0, 0+3) 等于
     * { (1,0), (1,1), (1,2), (2,0), (2,1), (2,2) }
     *
     * 一个子区域 [i0, i0+h) * [j0, j0+w) 的面积定义为它作为一个集合的序数，从而也就是 h*w.
     */
    class Solution {
    public:
        int maximalSquare(std::vector<std::vector<char>> matrix) {
            size_t N = matrix.size();
            assert((N >= 1));
            size_t M = matrix[0].size();
            std::vector<std::vector<int>> right (N);
            for (auto &row : right)
                row.resize(M, 0);
            std::vector<std::vector<int>> down (N);
            for (auto &row : down)
                row.resize(M, 0);

            int cMap[255];
            cMap['1'] = 1;
            cMap['0'] = 0;

            for (size_t i = 0, j = M-1; i < N; ++i)
                right[i][j] = cMap[matrix[i][j]];

            for (size_t j = 0, i = N-1; j < M; ++j)
                down[i][j] = cMap[matrix[i][j]];

            for (size_t di = 1; di < N; ++di) {
                for (size_t j = 0; j < M; ++j) {
                    size_t i = N-1-di;
                    if (matrix[i][j] == '1')
                        down[i][j] = down[i+1][j] + 1;
                }
            }

            for (size_t dj = 1; dj < M; ++dj) {
                for (size_t i = 0; i < N; ++i) {
                    size_t j = M-1-dj;
                    if (matrix[i][j] == '1')
                        right[i][j] = right[i][j+1] + 1;
                }
            }

            size_t maxSquareLen = std::min(N, M);
            std::vector<std::vector<std::vector<uint8_t>>> squareExists (N);
            for (auto &row : squareExists) {
                row.resize(M);
                for (auto &cell : row)
                    cell.resize(maxSquareLen, 0);
            }

            // squareExists[i][j][k] 表示矩阵的 [i, i+k+1) * [j, j+k+1) 区域内是否全部为 1
            size_t foundMaxLen = 0;
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < M; ++j) {
                    if (matrix[i][j] == '1') {
                        squareExists[i][j][0] = 1;
                        foundMaxLen = 1;
                    }
                }
            }

            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < M; ++j) {
                    // 注意：
                    // 这里的 maxPossibleLen 是长度单位，前面的 i, j 都是下标单位，要转变一下思维惯性。
                    size_t maxPossibleLen = std::min({maxSquareLen, N - i, M - j});
                    for (size_t len = 2; len <= maxPossibleLen; ++len) {
                        if (squareExists[i][j][len-2]) {
                            if (down[i][j+len-1] >= len && right[i+len-1][j] >= len) {
                                squareExists[i][j][len-1] = 1;
                                if (len > foundMaxLen) {
                                    foundMaxLen = len;
                                }
                                continue;
                            }
                        }
                        break;
                    }
                }
            }

            return static_cast<int>(foundMaxLen*foundMaxLen);
        }
    };

    struct TestCase {
        std::vector<std::vector<char>> matrix;
        int expectedOutput;
    };

    std::vector<TestCase> getTestCases() {
        return {
            TestCase {
                .matrix = {{'1','1'},{'1','1'}},
                .expectedOutput = 4
            },
            TestCase {
                .matrix = {{'1','0','1','0','0'},{'1','0','1','1','1'},{'1','1','1','1','1'},{'1','0','0','1','0'}},
                .expectedOutput = 4
            },
            TestCase {
                .matrix = {{'0','1'},{'1','0'}},
                .expectedOutput = 1
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMSQUARE_HPP
