//
// Created by 韦晓枫 on 2022/7/20.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_WILDCARDMATCH_H
#define DATASTRUCTUREIMPLEMENTATIONS_WILDCARDMATCH_H

#include <vector>
#include <cstdint>
#include <string>

namespace Algorithm {
    namespace WildcardMatch {

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

#endif //DATASTRUCTUREIMPLEMENTATIONS_WILDCARDMATCH_H
