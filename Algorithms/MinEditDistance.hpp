//
// Created by 韦晓枫 on 2022/7/23.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MINEDITDISTANCE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MINEDITDISTANCE_HPP

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

namespace Algorithm {
    namespace MinEditDistance {

        /**
         * 序列对比：
         *
         * 返回从序列 a 得到序列 b 所需的最小编辑步数，设 a 的长度是 N, b 的长度是 M,
         * 用元组 (i, j), 0 < i < N+1, 0 < j < M+1 表示编辑器的状态，则每一个原子编辑步骤是下述之一：
         *
         * - 删除 a[i], 状态更新到 (i+1, j), 这步的成本为 1,
         * - 插入 b[j], 状态更新到 (i, j+1), 这步的成本为 1,
         * - 用 b[j] 替换 a[i], 状态更新到 (i+1, j+1), 如果 b[j] == a[i], 这步的成本为 0, 否则这步的成本为 1.
         *
         * 编辑器一开始处在状态 (0,0), 我们要求的是编辑器从 (0,0) 走到 (N,M) 所需的最小成本，
         * 我们用函数 minCost(i,j) 表示编辑器从 (i,j) 走到 (N,M) 所需的最小成本。
         *
         * 易知，函数 minCost 具备下列性质：
         *
         * 1. minCost(N,M) == 0;
         * 2. minCost(i,j) == min{ 1+minCost(i+1,j), 1+minCost(i,j+1), cost(i,j)+minCost(i+1,j+1) };
         *
         * 性质 2 要求 0 <= i < N, 且 0 <= j < M, 并且式中：
         *
         * cost(i,j) == 0, 如果 a[i] == b[j], 否则 cost(i,j) == 1.
         *
         * 函数 minCost 的性质 1,2 立刻 leads to 一个 DP 的 solution 如下：
         *
         */
        template <typename SequenceItemType>
        size_t getMinEditDistance(SequenceItemType *a, SequenceItemType *b, size_t lenA, size_t lenB) {
            std::vector<std::vector<size_t>> minCost (lenA + 1);
            for (auto &row : minCost) {
                row.resize(lenB + 1);
            }


            auto printMinCostTable = [&minCost, a, b, lenA, lenB]() -> void {
                size_t maxAbsCost = 0;
                for (const auto &row : minCost) {
                    for (const auto &value : row) {
                        maxAbsCost = std::max(value, maxAbsCost);
                    }
                }
                double maxCost = maxAbsCost;
                double log10_float_t = std::log(maxCost) / std::log(10.0);
                int log10 = std::ceil(log10_float_t);

                int cellWidth = std::max(2, log10);
                std::cout << std::setw(cellWidth) << "";
                for (size_t j = 0; j < lenB; ++j) {
                    std::cout << std::setw(cellWidth) << b[j];
                }
                std::cout << "\n";

                for (size_t i = 0; i < lenA+1; ++i) {
                    if (i < lenA) {
                        std::cout << std::setw(cellWidth) << a[i];
                    } else {
                        std::cout << std::setw(cellWidth) << "";
                    }

                    for (size_t j = 0; j < lenB+1; ++j) {
                        std::cout << std::setw(cellWidth) << minCost[i][j];
                    }
                    std::cout << "\n";
                }
            };

            minCost[lenA][lenB] = 0;
            for (size_t di = 1; di < lenA+1; ++di) {
                minCost[lenA - di][lenB] = minCost[lenA - di + 1][lenB] + 1;
            }
            for (size_t dj = 1; dj < lenB+1; ++dj) {
                minCost[lenA][lenB - dj] = minCost[lenA][lenB - dj + 1] + 1;
            }

            // printMinCostTable();

            for (size_t di = 1; di < lenA+1; ++di) {
                for (size_t dj = 1; dj < lenB+1; ++dj) {
                    size_t i = lenA-di;
                    size_t j = lenB-dj;

                    size_t replacementCost = a[i] == b[j] ? 0 : 1;
                    minCost[i][j] = std::min({
                         minCost[i + 1][j + 1] + replacementCost,
                         minCost[i + 1][j] + 1,
                         minCost[i][j + 1] + 1
                    });
                }
            }

            // printMinCostTable();

            return minCost[0][0];
        }

        size_t getMinEditDistance(const std::string &a, const std::string &b) {
            return getMinEditDistance(a.data(), b.data(), a.size(), b.size());
        }

        struct TestCase {
             std::string lhs;
             std::string rhs;
             size_t expectedOutput;
        };

        std::vector<TestCase> getTestCases() {
            return {
                TestCase {
                    .lhs = "horse",
                    .rhs = "ros",
                    .expectedOutput = 3
                },
                TestCase {
                    .lhs = "intention",
                    .rhs = "execution",
                    .expectedOutput = 5
                }
            };
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MINEDITDISTANCE_HPP
