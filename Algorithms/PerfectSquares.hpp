//
// Created by 韦晓枫 on 2022/7/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_PERFECTSQUARES_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_PERFECTSQUARES_HPP

#include <cmath>
#include <vector>
#include <algorithm>

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
                TestCase { .n = 8328, .expected = 1 }
            };
        }

        class Solution {
        public:
            Solution( ) {
                this->memo.resize(this->maxPrefix);
                for (size_t i = 0; i < this->maxPrefix; ++i) {
                    this->memo[i].resize(this->maxN+1);
                }

                for (int n = 0, h = 0; n <= this->maxN; ++n) {
                    this->updateMemo(h, n);
                }
            }

            int numSquares(int n) {
                double n_float = n;
                double n_squareRoot = sqrt(n_float);
                int n_floor = floor(n_squareRoot);
                if (n_floor > this->maxPrefix || n > this->maxN) {
                    this->growMemo(std::max(n_floor, this->maxPrefix), std::max(n, this->maxN));
                }

                return this->memo[n_floor-1][n];
            }

        private:
            int maxPrefix = 1;
            int maxN = 1;
            std::vector<std::vector<int>> memo;

            void growMemo(int newMaxPrefix, int newN) {
                this->memo.resize(newMaxPrefix);
                for (size_t h = 0; h < newMaxPrefix; ++h) {
                    this->memo[h].resize(newN+1);
                }

                for (int n = 0; n <= this->maxN; ++n) {
                    for (int h = this->maxPrefix; h < newMaxPrefix; ++h) {
                        this->updateMemo(h, n);
                    }
                }

                this->maxPrefix = newMaxPrefix;

                for (int n = this->maxN+1; n <= newN; ++n) {
                    for (int h = 0; h < this->maxPrefix; ++h) {
                        this->updateMemo(h, n);
                    }
                }

                this->maxN = newN;
            }

            void updateMemo(int h, int n) {
                if (h == 0) {
                    this->memo[h][n] = n;
                    return;
                }

                if (n == 0) {
                    this->memo[h][n] = 0;
                    return;
                }

                int answer = this->memo[h-1][n];
                int cnt = 1;
                int remains = n - cnt * (h+1) * (h+1);
                while (remains >= 0) {
                    answer = std::min(answer, cnt + this->memo[h][remains]);

                    ++cnt;
                    remains = n - cnt * (h+1) * (h+1);
                }
                this->memo[h][n] = answer;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_PERFECTSQUARES_HPP
