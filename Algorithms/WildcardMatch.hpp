//
// Created by 韦晓枫 on 2022/7/20.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_WILDCARDMATCH_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_WILDCARDMATCH_HPP

#include <vector>
#include <cstdint>
#include <string>

namespace Algorithm {
    namespace WildcardMatch {

        /**
         * 正则匹配问题：
         *
         * 问题描述：
         * 给你一个 const char *s 代表字符串，const char *p 代表正则符号序列（正则表达式也就是说），
         * s 的长度是 N, p 的长度是 M,
         * 试判定 s 与 p 是否匹配。
         *
         * 思路：
         * 定义函数：
         * isMatch(sequenceLength: int, patternLength: int) -> bool
         * 表示子串 s[0..sequenceLength] 与子 pattern p[0..patternLength] 是否匹配。
         *
         * 我们从 size_t i = 0, j = 0; 开始，知道 isMatch(i, j) = 1;
         * 然后我们一步一步增大 i 和 j, 用 DP 的方式构筑越来越大的 i, j 的 isMatch(i, j) 值。
         *
         * 最终算出的 isMatch(N, M) 就是我们想要的答案。
         */

        using std::string;

        class Solution {
        private:
            bool doCheckIsMatch(const char *s, const char *p, size_t N, size_t M) {
                std::vector<std::vector<uint8_t>> match;
                match.resize(N+1);
                for (auto &row : match) {
                    row.resize(M+1, 0);
                }

                match[0][0] = 1;
                for (size_t j = 1; j <= M; ++j) {
                    if (p[j-1] == '?') {
                        for (size_t i = 1; i <= N; ++i) {
                            match[i][j] = match[i-1][j-1];
                        }
                    } else if (p[j-1] == '*') {
                        for (size_t i = 0; i <= N; ++i) {
                            if (match[i][j-1]) {
                                for (size_t n = 0; i+n <= N; ++n) {
                                    match[i+n][j] = 1;
                                }
                                break;
                            }
                        }
                    } else {
                        for (size_t i = 1; i <= N; ++i) {
                            match[i][j] = s[i-1] == p[j-1] && match[i-1][j-1] == 1;
                        }
                    }
                }

                return match[N][M];
            }

        public:
            bool isMatch(string s, string p) {
                return doCheckIsMatch(s.data(), p.data(), s.size(), p.size());
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_WILDCARDMATCH_HPP
