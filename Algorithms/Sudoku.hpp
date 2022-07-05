//
// Created by 韦晓枫 on 2022/7/6.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SUDOKU_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SUDOKU_HPP

#include <bitset>
#include <vector>

namespace Algorithm {
    /** 合法数独校验算法实现 */
    namespace ValidSudoku {

        /**
         * 合法数独校验问题：
         *
         * 用一个 board: vector<vector<char>>, 规格为 9 x 9 表示数独棋盘，
         * 用字符 '1' 到 '9' 表示阿拉伯数字 1 到 9, 用 '.' 表示没填，判定该数独是否是合法数独。
         *
         * 定义：
         * 一个合法数独每行、每列、每个 sub-box 都不应该有重复数字，且每个单元格上或者没有填数字，
         * 或者只能填一个 1 到 9 的数字（包括 1 和 9）。
         *
         * 解法：
         * 我们主要是想尝试用 std::bitset 来记录和判定一个行、列或者 sub-box 的数字填写情况，确保没有重复的。
         *
         */

        bool validSudoku(const std::vector<std::vector<char>> &board) {
            // 期待编译器自动 inline 这个 lambda 吧，虽然我也很不确定。
            auto charToInteger = [](char c) -> size_t {
                return static_cast<size_t>(static_cast<uint8_t>(c));
            };

            // 提些问题给诸位：
            // 我栈上分配这么多不用到的空间：
            // (1) 会显著影响程序的 locality 吗？
            // (2) 会导致缺页错误数量激增吗？
            // (3) 会造成 CPU 缓存脱靶次数激增吗？
            // (4) 会影响程序性能吗？
            size_t charToIdx[256];
            charToIdx[charToInteger('1')] = 1;
            charToIdx[charToInteger('2')] = 2;
            charToIdx[charToInteger('3')] = 3;
            charToIdx[charToInteger('4')] = 4;
            charToIdx[charToInteger('5')] = 5;
            charToIdx[charToInteger('6')] = 6;
            charToIdx[charToInteger('7')] = 7;
            charToIdx[charToInteger('8')] = 8;
            charToIdx[charToInteger('9')] = 9;

            // 一行一行检查
            constexpr size_t nRows = 9;
            constexpr size_t nCols = 9;
            for (size_t rowIdx = 0; rowIdx < nRows; ++rowIdx) {
                std::bitset<9> presents(0);
                for (size_t colIdx = 0; colIdx < nCols; ++colIdx) {
                    char c = board[rowIdx][colIdx];
                    if (c == '.')
                        continue;

                    size_t cellNumber = charToIdx[charToInteger(c)]-1;
                    if (presents.test(cellNumber)) {
                        return false;
                    }
                    presents[cellNumber] = true;
                }
            }

            // 一列一列检查
            for (size_t colIdx = 0; colIdx < nCols; ++colIdx) {
                std::bitset<9> presents(0);
                for (size_t rowIdx = 0; rowIdx < nRows; ++rowIdx) {
                    char c = board[rowIdx][colIdx];
                    if (c == '.')
                        continue;

                    size_t cellNumber = charToIdx[charToInteger(c)]-1;
                    if (presents.test(cellNumber)) {
                        return false;
                    }
                    presents[cellNumber] = true;
                }
            }

            constexpr size_t subBoxesCount = 9;
            constexpr size_t subBoxEdgeLen = 3;
            size_t subBoxRowOffsets[subBoxesCount] = { 0, 0, 0, 3, 3, 3, 6, 6, 6 };
            size_t subBoxColOffsets[subBoxesCount] = { 0, 3, 6, 0, 3, 6, 0, 3, 6 };
            for (size_t subBoxIdx = 0; subBoxIdx < subBoxesCount; ++subBoxIdx) {
                size_t rowOffset = subBoxRowOffsets[subBoxIdx];
                size_t colOffset = subBoxColOffsets[subBoxIdx];
                std::bitset<9> presents(0);
                for (size_t i = 0; i < subBoxEdgeLen; ++i) {
                    for (size_t j = 0; j < subBoxEdgeLen; ++j) {
                        size_t rowIdx = rowOffset+i;
                        size_t colIdx = colOffset+j;

                        char c = board[rowIdx][colIdx];
                        if (c == '.')
                            continue;

                        size_t cellNumber = charToIdx[charToInteger(c)]-1;
                        if (presents.test(cellNumber)) {
                            return false;
                        }
                        presents[cellNumber] = true;
                    }
                }
            }

            return true;
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SUDOKU_HPP
