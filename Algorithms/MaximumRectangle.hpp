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

                int charToNum[256];
                charToNum['1'] = 1;
                charToNum['0'] = 0;

                size_t nRows = matrix.size();
                size_t nCols = matrix[0].size();
                std::vector<std::vector<Cell>> cells (nRows);
                for (auto &row : cells) {
                    row.resize(nCols);
                }

                auto &corner = cells[nRows-1][nCols-1];
                int cornerDigit = charToNum[matrix[nRows-1][nCols-1]];
                corner.right = cornerDigit;
                corner.down = cornerDigit;

                for (size_t offset = 1; offset < nRows; ++offset) {
                    size_t i = nRows-1-offset;
                    size_t j = nCols-1;
                    cells[i][j].right = charToNum[matrix[i][j]];
                    cells[i][j].down = cells[i][j].right * (1+cells[i+1][j].down);
                }

                for (size_t offset = 1; offset < nCols; ++offset) {
                    size_t i = nRows-1;
                    size_t j = nCols-1-offset;
                    cells[i][j].down = charToNum[matrix[i][j]];
                    cells[i][j].right = cells[i][j].down * (1+cells[i][j+1].right);
                }

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

                size_t maxArea = 0;
                for (size_t nCol = 1; nCol <= nCols; ++nCol) {
                    for (size_t nRow = 1; nRow <= nRows; ++nRow) {
                        for (size_t i = 0; i <= nRows-nRow; ++i) {
                            for (size_t j = 0; j <= nCols-nCol; ++j) {
                                if (isRect(cells, i, j, nCol, nRow)) {
                                     maxArea = std::max(maxArea, nRow*nCol);
                                }
                            }
                        }
                    }
                }

                return static_cast<int>(maxArea);
            }

        private:

            struct Cell {
                int right;
                int down;
            };

            /**
             * 若 cells 矩阵的矩形区域 [i, i+h) x [j, j+w) 内全是 1, 则返回 true, 否则返回 false
             */
            bool isRect(
                    const vector<vector<Cell>> &cells,
                    size_t i,
                    size_t j,
                    size_t w,
                    size_t h
            ) {

                if (h == 1) {
                    return cells[i][j].right >= w;
                } else if (w == 1) {
                    return cells[i][j].down >= h;
                } else if (h >= 2 && w >= 2) {
                    return isRect(cells, i, j, 1, h) &&
                            isRect(cells, i, j, w, 1) &&
                            isRect(cells, i+1, j+1, w-1, h-1);
                } else {
                    assert(false);
                }
            }
        };
    }
}



#endif //DATASTRUCTUREIMPLEMENTATIONS_MAXIMUMRECTANGLE_HPP
