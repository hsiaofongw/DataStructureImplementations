//
// Created by 韦晓枫 on 2022/7/23.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_DISTINCTSUBSEQUENCES_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_DISTINCTSUBSEQUENCES_HPP

#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>

namespace Algorithm {
    namespace DistinctSubsequences {

        struct TestCase {
            std::string originString;
            std::string subSequence;
            uint32_t desiredOutput;
        };

        std::vector<TestCase> getTestCases() {
            return {
                TestCase {
                    .originString = "rabbbit",
                    .subSequence = "rabbit",
                    .desiredOutput = 3
                },
                TestCase {
                    .originString = "babgbag",
                    .subSequence = "bag",
                    .desiredOutput = 5
                }
            };
        }

        uint32_t numDistinct(const std::string &a, const std::string &b) {
            // 用 count[i][j] 表示 a 的 i 长度后缀中有多少个不同的 b 的 j 长度后缀子序列。
            size_t N = a.size(), M = b.size();
            std::vector<std::vector<uint32_t>> count (N+1);
            for (auto &row : count) {
                row.resize(M+1, 0);
            }

            // 计算 count[...][0]
            for (size_t i = 0; i <= N; ++i) {
                count[i][0] = 1;
            }

            // 计算 count[...][j], j = 1,2,...,M
            const char *strA = a.data();
            const char *strB = b.data();
            for (size_t j = 1; j <= M; ++j) {
                for (size_t i = 0; i <= N; ++i) {
                    if (i < j) {
                        count[i][j] = 0;
                    } else if (i > j) {
                        if (strA[N-i] == strB[M-j]) {
                            count[i][j] = count[i-1][j-1] + count[i-1][j];
                        } else {
                            count[i][j] = count[i-1][j];
                        }
                    } else {
                        // i == j
                        if (strA[N-i] == strB[M-j]) {
                            count[i][j] = count[i-1][j-1];
                        } else {
                            count[i][j] = 0;
                        }
                    }
                }
            }

            return count[N][M];
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_DISTINCTSUBSEQUENCES_HPP
