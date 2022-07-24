//
// Created by 韦晓枫 on 2022/7/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_PERFECTSQUARES_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_PERFECTSQUARES_HPP

#include <cmath>
#include <vector>
#include <algorithm>
#include <array>

namespace Algorithm {
    namespace PerfectSquare {
        /**
         * 完全平方数问题：
         *
         * 一个整数 n 如果它配被表为一个整数 p 的平方，则称 n 是一个完全平方数。
         *
         * 例如：
         * 因为 1 = 1^2, 4 = 2^2, 9 = 3^2, 16 = 4^2, 所以 1,4,9,16 都是完全平方数。
         *
         * 给定自然数 n, 试找到一组最少数量的完全平方数 n_1, n_2, ..., n_M 满足
         * n_1 + n_2 + ... + n_M == n.
         *
         * 提示，n_i, i=1,2,...,M 中可以有重复的，并且这样的一组数都是存在的。
         */

        struct TestCase {
            int n;
            int expected;
        };

        std::vector<TestCase> getTestCases() {
            return {
                TestCase { .n = 12, .expected = 3 },
                TestCase { .n = 13, .expected = 2 },
                TestCase { .n = 8328, .expected = 3 },
                TestCase { .n = 5374, .expected = 3 }
            };
        }

        template<int N = 10000, int M = 100>
        consteval std::array<std::array<int, N+1>, M> numPerfectSquaresMemo() {
            std::array<std::array<int, N+1>, M> memo;
            for (size_t i = 0, j = 0; j <= N; ++j) {
                memo[i][j] = j;
            }

            for (size_t i = 1, j = 0; i < M; ++i) {
                memo[i][j] = 0;
            }

            for (size_t j = 1; j <= N; ++j) {
                for (size_t i = 1; i < M; ++i) {
                    memo[i][j] = memo[i-1][j];
                    int cnt = 1;
                    int remains = j - cnt * (i+1) * (i+1);
                    while (remains >= 0) {
                        memo[i][j] = std::min(memo[i][j], cnt + memo[i][remains]);
                        ++cnt;
                        remains = j - cnt * (i+1) * (i+1);
                    }
                }
            }

            return memo;
        }

        int numPerfectSquares(int n) {
            double n_float = n;
            double n_squareRoot = sqrt(n_float);
            int n_floor = floor(n_squareRoot);
            std::vector<std::vector<int>> memo (n_floor);
            for (auto &row : memo) {
                row.resize(n+1);
            }

            size_t N = n;
            size_t M = n_floor;

            for (size_t i = 0, j = 0; j <= N; ++j) {
                memo[i][j] = j;
            }

            for (size_t i = 1, j = 0; i < M; ++i) {
                memo[i][j] = 0;
            }

            for (size_t j = 1; j <= N; ++j) {
                for (size_t i = 1; i < M; ++i) {
                    int ans = memo[i-1][j];
                    int cnt = 1;
                    int remains = j - cnt * (i+1) * (i+1);
                    if (remains >= 0) {
                        int newAns = cnt + memo[i][remains];
                        if (newAns < ans) {
                            ans = newAns;
                        }
                    }

                    memo[i][j] = ans;
                }
            }

            return memo[n_floor-1][n];
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_PERFECTSQUARES_HPP
