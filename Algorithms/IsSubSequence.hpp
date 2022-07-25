//
// Created by 韦晓枫 on 2022/7/25.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_ISSUBSEQUENCE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_ISSUBSEQUENCE_HPP

#include <string>
#include <vector>

namespace Algorithm {
    namespace IsSubSequence {

        struct TestCase {
            std::string subSequence;
            std::string text;
            bool expectedOutput;
        };

        std::vector<TestCase> getTestCases() {
            return {
                    TestCase {
                        .subSequence = "abc",
                        .text = "ahbgdc",
                        .expectedOutput = true
                    },
                    TestCase {
                        .subSequence = "axc",
                        .text = "ahbgdc",
                        .expectedOutput = false
                    },
                    TestCase {
                        .subSequence = "abc",
                        .text = "",
                        .expectedOutput = false
                    }
            };
        }

        /**
         * 子序列判定问题
         */
        class Solution {
        public:
            bool isSubsequence(std::string subSequence, std::string text) {
                size_t N = text.size();
                size_t M = subSequence.size();
                std::vector<std::vector<uint8_t>> memo (N+1);
                for (auto &row : memo) {
                    row.resize(M+1, 0);
                }

                // memo[i][j] 表示 subSequence 串的 j 长后缀是否是 text 串的 i 长后缀的子序列，
                // 其中，i 的取值在闭区间 [0, N], j 的取值在闭区间 [0, M]
                for (size_t j = 0, i = 0; i <= N; ++i) {
                    // subSequence 串的 0 长后缀是 text 串的任何 i 长后缀的子序列
                    memo[i][j] = 1;
                }

                // 依次计算：memo[0][1], memo[1][1], ..., memo[N][1],
                //         memo[0][2], memo[1][2], ..., memo[N][2],
                //         ...
                //         memo[0][M], memo[1][M], ..., memo[N][M]
                for (size_t j = 1; j <= M; ++j) {
                    for (size_t i = 0; i <= N; ++i) {
                        if (i < j) {
                            memo[i][j] = 0;
                        } else if (i > j) {
                            if (text[N-i] == subSequence[M-j]) {
                                memo[i][j] = memo[i-1][j-1] == 1 || memo[i-1][j] == 1;
                            } else {
                                memo[i][j] = memo[i-1][j];
                            }
                        } else {
                            // i == j
                            memo[i][j] = text[N-i] == subSequence[M-j] && memo[i-1][j-1] == 1;
                        }
                    }
                }

                return memo[N][M] == 1;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_ISSUBSEQUENCE_HPP
