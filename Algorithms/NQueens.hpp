//
// Created by 韦晓枫 on 2022/7/7.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_NQUEENS_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_NQUEENS_HPP

#include <iostream>
#include <bitset>
#include <vector>
#include <list>
#include <cmath>
#include <memory>

namespace Algorithm {

    namespace NQueens {
        // 坐标
        struct Point {
            size_t rowIdx;
            size_t colIdx;
        };

        // 棋盘状态
        template<size_t MaxBoardSize = 32>
        struct Board {
            explicit Board(const size_t problemSizeN) : currentProblemSize(problemSizeN) {}
            std::bitset<MaxBoardSize> rows; // rows[i] 表示下标为 i 的行是否占用
            std::bitset<MaxBoardSize> cols; // cols[i] 表示下标为 i 的列是否占用
            std::bitset<2 * MaxBoardSize - 1> pDiags; // pDiags[i] 表示 rowIdx + colIdx == i 的正对角线是否占用
            std::bitset<2 * MaxBoardSize - 1> nDiags; // nDiags[i] 表示 rowIdx - colIdx + currentProblemSize-1 == i 的负对角线是否占用
            std::list<Point> points; // 点列表，最新的在最后
            size_t currentProblemSize = 8; // 当前的 ProblemSizeN, 当前要处理的问题的规模
        };

        // 写
        template<size_t N>
        void writeBoard(Board<N> &board, const Point &p) {
            board.rows.set(p.rowIdx, true);
            board.cols.set(p.colIdx, true);
            board.pDiags.set(p.rowIdx + p.colIdx, true);
            board.nDiags.set(p.rowIdx - p.colIdx + board.currentProblemSize - 1, true);
            board.points.push_back(p);
        }

        // 删
        template<size_t N>
        void unsetBoard(Board<N> &board, const Point &p) {
            board.rows.set(p.rowIdx, false);
            board.cols.set(p.colIdx, false);
            board.pDiags.set(p.rowIdx + p.colIdx, false);
            board.nDiags.set(p.rowIdx - p.colIdx + board.currentProblemSize - 1, false);
            board.points.pop_back();
        }

        // 测试
        template<size_t N>
        bool testFeasibility(Board<N> &board, const Point &p) {
            if (board.rows.test(p.rowIdx)) {
                return false;
            }

            if (board.cols.test(p.colIdx)) {
                return false;
            }

            if (board.pDiags.test(p.rowIdx + p.colIdx)) {
                return false;
            }

            if (board.nDiags.test(p.rowIdx - p.colIdx + board.currentProblemSize - 1)) {
                return false;
            }

            return true;
        }

        template<size_t N>
        void doGenerateSolutions(
                std::list<size_t> &letters,
                Board<N> &board,
                std::vector<std::vector<Point>> &solutions,
                size_t level
        ) {

            if (level == board.currentProblemSize) {
                std::vector<Point> points{};
                for (const auto &point: board.points) {
                    points.push_back(point);
                }
                solutions.push_back(std::move(points));
                return;
            }

            size_t choices = letters.size();
            for (size_t i = 0; i < choices; ++i) {
                Point p {};
                p.rowIdx = letters.front();
                p.colIdx = level;
                letters.pop_front();
                if (testFeasibility(board, p)) {
                    writeBoard(board, p);
                    doGenerateSolutions(letters, board, solutions, level + 1);
                    unsetBoard(board, p);
                }
                letters.push_back(p.rowIdx);
            }
        }

        std::vector<std::vector<Point>> getNQueensSolutions(const size_t problemSizeN) {
            Board<> board (problemSizeN);
            std::vector<std::vector<Point>> solutions {};
            std::list<size_t> letters {};
            for (size_t i = 0; i < problemSizeN; ++i) {
                letters.push_back(i);
            }
            doGenerateSolutions(letters, board, solutions, 0);
            return solutions;
        }

        std::string solutionToString(const std::vector<Point> &solution, const size_t problemSizeN) {
            char solutionView[problemSizeN + 2][problemSizeN + 3];
            for (size_t i = 0; i < problemSizeN + 2; ++i) {
                for (size_t j = 0; j < problemSizeN + 2; ++j) {
                    solutionView[i][j] = ' ';
                }
            }

            for (size_t i = 0; i < problemSizeN + 2; ++i) {
                solutionView[i][0] = '*';
                solutionView[i][problemSizeN + 2 - 1] = '*';
                solutionView[0][i] = '*';
                solutionView[problemSizeN + 2 - 1][i] = '*';
                solutionView[i][problemSizeN + 3 - 1] = '\n';
            }

            for (const auto &point: solution) {
                solutionView[point.rowIdx + 1][point.colIdx + 1] = 'Q';
            }

            std::string viewStr;
            for (size_t i = 0; i < problemSizeN + 2; ++i) {
                for (size_t j = 0; j < problemSizeN + 3; ++j) {
                    viewStr.push_back(solutionView[i][j]);
                }
            }

            return viewStr;
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_NQUEENS_HPP
