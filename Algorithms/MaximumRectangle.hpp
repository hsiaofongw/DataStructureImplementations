//
// Created by 韦晓枫 on 2022/7/13.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMRECTANGLE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMRECTANGLE_HPP

#include <vector>
#include <algorithm>
#include "../Interfaces/Matrix.hpp"

namespace Algorithm {
    namespace MaximumRectangle {

        using std::vector;
        using Matrix = Interface::Matrix<char>;
        using TestCase = std::pair<Matrix, int>;
        using TestCases = std::vector<TestCase>;

        TestCases getTestCases() {
            return {
                    {{{'1','0','1','0','0'},{'1','0','1','1','1'},{'1','1','1','1','1'},{'1','0','0','1','0'}}, 6},
                    {{{'0'}}, 0},
                    {{{'1'}}, 1},
                    {{{'1','0'},{'1','0'}}, 2},
                    {{{'1','1','1','1','1','1','1','1'},{'1','1','1','1','1','1','1','0'},{'1','1','1','1','1','1','1','0'},{'1','1','1','1','1','0','0','0'},{'0','1','1','1','1','0','0','0'}}, 21}
            };
        }

        class Solution {
        public:
            int maximalRectangle(vector<vector<char>>& matrix) {
                struct Cell {
                    int right;
                    int down;
                    int rectWidth;
                    int rectHeight;
                };

                int charToNum[256];
                charToNum['1'] = 1;
                charToNum['0'] = 0;

                size_t nRows = matrix.size();
                size_t nCols = matrix[0].size();
                std::vector<std::vector<Cell>> cells (nRows);
                for (auto &row : cells) {
                    row.resize(nCols);
                }

                // 计算 f[N-1][M-1]
                auto &corner = cells[nRows-1][nCols-1];
                int cornerDigit = charToNum[matrix[nRows-1][nCols-1]];
                corner.right = cornerDigit;
                corner.down = cornerDigit;
                corner.rectHeight = cornerDigit;
                corner.rectWidth = cornerDigit;

                // 计算 f[...][M-1]（除 f[N-1][M-1] 之外的）
                for (size_t offset = 1; offset < nRows; ++offset) {
                    size_t i = nRows-1-offset;
                    size_t j = nCols-1;
                    cells[i][j].right = charToNum[matrix[i][j]];
                    cells[i][j].down = cells[i][j].right * (1+cells[i+1][j].down);
                    cells[i][j].rectWidth = cells[i][j].right;
                    cells[i][j].rectHeight = cells[i][j].down;
                }

                // 计算 f[N-1][...]（除 f[N-1][M-1] 之外的）
                for (size_t offset = 1; offset < nCols; ++offset) {
                    size_t i = nRows-1;
                    size_t j = nCols-1-offset;
                    cells[i][j].down = charToNum[matrix[i][j]];
                    cells[i][j].right = cells[i][j].down * (1+cells[i][j+1].right);
                    cells[i][j].rectHeight = cells[i][j].down;
                    cells[i][j].rectWidth = cells[i][j].right;
                }

                // 计算 f[...][...] 之前先计算 down 和 right 字段的值：每个 cell 向下，向右的 1 延展的长度
                for (size_t iOffset = 1; iOffset < nRows; ++iOffset) {
                    for (size_t jOffset = 1; jOffset < nCols; ++jOffset) {
                        size_t i = nRows-1-iOffset;
                        size_t j = nCols-1-jOffset;
                        int currentDigit = charToNum[matrix[i][j]];
                        cells[i][j].right = currentDigit * (1+cells[i][j+1].right);
                    }
                }
                for (size_t jOffset = 1; jOffset < nCols; ++jOffset) {
                    for (size_t iOffset = 1; iOffset < nRows; ++iOffset) {
                        size_t i = nRows-1-iOffset;
                        size_t j = nCols-1-jOffset;
                        int currentDigit = charToNum[matrix[i][j]];
                        cells[i][j].down = currentDigit * (1+cells[i+1][j].down);
                    }
                }

                for (size_t iOffset = 1; iOffset < nRows; ++iOffset) {
                    for (size_t jOffset = 1; jOffset < nCols; ++jOffset) {
                        size_t i = nRows-1-iOffset;
                        size_t j = nCols-1-jOffset;
                        if (cells[i][j].right != 0) {
                            // 这其实和 cells[i][j].down != 0 是一样的

                            auto &cell = cells[i][j];
                            cell.rectWidth = 0;
                            cell.rectHeight = 0;
                            const auto &rightNeighbor = cells[i][j+1];
                            const auto &downNeighbor = cells[i+1][j];

                            // candidate solution 1
                            int h1 = cell.down;
                            int w1 = 1;

                            // candidate solution 2
                            int h2 = std::min(cell.down, rightNeighbor.rectHeight);
                            int w2 = 1 + rightNeighbor.rectWidth;

                            // candidate solution 3
                            int h3 = 1;
                            int w3 = cell.right;

                            // candidate solution 4
                            int h4 = 1 + downNeighbor.rectHeight;
                            int w4 = std::min(cell.right, downNeighbor.rectWidth);

                            // find area maximal within candidates
                            auto updateDim = [&cell](int h, int w) -> void {
                                if (h*w > cell.rectHeight*cell.rectWidth) {
                                    cell.rectHeight = h;
                                    cell.rectWidth = w;
                                }
                            };
                            updateDim(h1, w1);
                            updateDim(h2, w2);
                            updateDim(h3, w3);
                            updateDim(h4, w4);
                        }
                    }
                }

                int maxArea = 0;
                for (size_t i = 0; i < nRows; ++i)
                    for (size_t j = 0; j < nCols; ++j)
                        maxArea = std::max(maxArea, cells[i][j].rectWidth*cells[i][j].rectHeight);

                return maxArea;
            }
        };
    }
}



#endif //DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMRECTANGLE_HPP
