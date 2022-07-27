//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SETMATRIXZEROES_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SETMATRIXZEROES_HPP

#include <vector>
#include <iostream>

namespace Algorithm::SetMatrixZeroes {
    class Solution {
    public:
        void setZeroes(std::vector<std::vector<int>> &matrix) {
            // 先按行置 0, 行内原先的 0 放到最下边。
            size_t N = matrix.size();
            assert((N >= 1));
            size_t M = matrix[0].size();

            // 提前收集好最后以后是否需要置 0 的信息，这样就可以放心覆写最后一行了，这样就不会丢失信息了。
            bool shouldZeroLastRow = false;
            for (size_t j = 0; j < M; ++j) {
                if (matrix[N-1][j] == 0) {
                    shouldZeroLastRow = true;
                    break;
                }
            }

            // std::cout << "zeroLastLine: " << std::boolalpha << shouldZeroLastRow << "\n";

            // 分别置 0 每一行（除了最后一行），如果该行有 0, 把该行的 0 放到最后一行
            for (size_t i = 0; i < N-1; ++i) {
                bool thisLineHasZero = false;
                for (size_t j = 0; j < M; ++j) {
                    if (matrix[i][j] == 0) {
                        thisLineHasZero = true;
                        matrix[N-1][j] = 0;
                    }
                }
                if (thisLineHasZero) {
                    for (size_t j = 0; j < M; ++j) {
                        matrix[i][j] = 0;
                    }
                }
            }

            // 在最后一行，判定哪些列需要置 0
            for (size_t j = 0; j < M; ++j) {
                if (matrix[N-1][j] == 0) {
                    for (size_t i = 0; i < N; ++i) {
                        matrix[i][j] = 0;
                    }
                }
            }

            // 如果之前判断得出最后一行需要按行置 0, 则对最后一行按行置 0
            if (shouldZeroLastRow)
                for (size_t j = 0; j < M; ++j)
                    matrix[N-1][j] = 0;
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SETMATRIXZEROES_HPP
