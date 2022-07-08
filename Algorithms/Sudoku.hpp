//
// Created by 韦晓枫 on 2022/7/6.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SUDOKU_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SUDOKU_HPP

#include <vector>
#include <iostream>
#include <bitset>
#include <list>
#include <cmath>
#include <memory>
#include <array>
#include <stack>


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

    /** 数独求解 */

    namespace SudokuSolve {

        /** 后续这些子 namespace 会拆分到不同到文件中，故而重复声明。 */
        struct Point {
            size_t rowIdx;
            size_t colIdx;
        };

        template <size_t N = 9>
        struct SudokuState {
            std::array<std::bitset<N>, N> rowBitmaps;
            std::array<std::bitset<N>, N> colBitmaps;
            std::array<std::bitset<N>, N> subBoxBitmaps;
        };

        using Board = std::vector<std::vector<uint8_t>>;

        template <size_t N = 9>
        bool doSolveSudoku(
                Board &board,
                SudokuState<> &state,
                std::stack<Point> &freeSlots,
                std::vector<Board> &solutions,
                bool stopAtFirstSolutionFound
        ) {
            if (freeSlots.empty()) {
                solutions.emplace_back(board);
                return true;
            }

            constexpr size_t subBoxIdxMap[N][N] = {
                    { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
                    { 0, 0, 0, 1, 1, 1, 2, 2, 2 },
                    { 0, 0, 0, 1, 1, 1, 2, 2, 2 },

                    { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
                    { 3, 3, 3, 4, 4, 4, 5, 5, 5 },
                    { 3, 3, 3, 4, 4, 4, 5, 5, 5 },

                    { 6, 6, 6, 7, 7, 7, 8, 8, 8 },
                    { 6, 6, 6, 7, 7, 7, 8, 8, 8 },
                    { 6, 6, 6, 7, 7, 7, 8, 8, 8 },
            };

            auto testCandidate = [&state, &subBoxIdxMap](const Point &position, const size_t candidate) -> bool {
                size_t subBoxIdx = subBoxIdxMap[position.rowIdx][position.colIdx];
                return !(
                        state.rowBitmaps[position.rowIdx].test(candidate) ||
                        state.colBitmaps[position.colIdx].test(candidate) ||
                        state.subBoxBitmaps[subBoxIdx].test(candidate)
                );
            };

            auto writeBoard = [&board, &state, &subBoxIdxMap](const Point &position, const size_t digit, bool value) -> void {
                // digit is 0-based
                board[position.rowIdx][position.colIdx] = value ? digit + 1 : 0;
                state.rowBitmaps[position.rowIdx].set(digit, value);
                state.colBitmaps[position.colIdx].set(digit, value);
                size_t subBoxIdx = subBoxIdxMap[position.rowIdx][position.colIdx];
                state.subBoxBitmaps[subBoxIdx].set(digit, value);
            };

            auto freeSlot = freeSlots.top();
            for (size_t candidate = 0; candidate < N; ++candidate) {
                if (testCandidate(freeSlot, candidate)) {
                    writeBoard(freeSlot, candidate, true);
                    freeSlots.pop();
                    bool isValidSolution = doSolveSudoku(board, state, freeSlots, solutions, stopAtFirstSolutionFound);
                    if (isValidSolution) {
                        if (stopAtFirstSolutionFound) {
                            return isValidSolution;
                        }
                    }
                    writeBoard(freeSlot, candidate, false);
                    freeSlots.push(freeSlot);
                }
            }

            return false;
        }

        template <size_t N = 9>
        void solveSoduku(Board &board) {
            SudokuState<> state;
            std::stack<Point> unFilledSlots;
            for (size_t i = 0; i < N; ++i) {
                for (size_t j = 0; j < N; ++j) {
                    if (board[i][j] == 0) {
                        Point p {i, j };
                        unFilledSlots.push(p);
                    }
                }
            }

            std::vector<Board> solutions {};
            doSolveSudoku<>(board, state, unFilledSlots, solutions, true);
        }

        std::string boardToStr(const Board &board) {
            size_t len = 1+3+1+3+1+3+1;
            char boardView[len][len];
            for (size_t i = 0; i < len; ++i) {
                boardView[i][0] = '*';
                boardView[i][4] = '*';
                boardView[i][8] = '*';
                boardView[i][len-1] = '*';

                boardView[0][i] = '*';
                boardView[4][i] = '*';
                boardView[8][i] = '*';
                boardView[len-1][i] = '*';
            }

            auto digitToChar = [](uint8_t digit) -> char {
                char digitChars[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
                return digitChars[static_cast<size_t>(digit)];
            };

            auto myMemMap = [&board, &boardView, &digitToChar](size_t fromI, size_t fromJ, size_t toI, size_t toJ) -> void {
                for (size_t i = 0; i < 3; ++i) {
                    for (size_t j = 0; j < 3; ++j) {
                        char c = digitToChar(board[fromI+i][fromJ+j]);
                        boardView[toI+i][toJ+j] = c;
                    }
                }
            };

            myMemMap(0, 0, 1, 1);
            myMemMap(0, 3, 1, 5);
            myMemMap(0, 6, 1, 9);
            myMemMap(3, 0, 5, 1);
            myMemMap(3, 3, 5, 5);
            myMemMap(3, 6, 5, 9);
            myMemMap(6, 0, 9, 1);
            myMemMap(6, 3, 9, 5);
            myMemMap(6, 6, 9, 9);

            std::string result;
            for (size_t i = 0; i < len; ++i) {
                std::string row;
                for (size_t j = 0; j < len; ++j) {
                    row += boardView[i][j];
                }
                row += '\n';
                result += row;
            }

            return result;
        }
    }

}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SUDOKU_HPP
