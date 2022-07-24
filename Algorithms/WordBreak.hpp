//
// Created by 韦晓枫 on 2022/7/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_WORDBREAK_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_WORDBREAK_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace Algorithm {
    namespace WordBreak {
        /**
         * WordBreak 问题：
         *
         * 判定一个 std::string s 能否由一个 std::vector<std::string> wordDict 中的一个或多个 word 前后拼接而成。
         *
         * 举例：
         *
         * s = "leetcode", wordDict = { "leet", "code" };
         * 返回 true, 因为 "leet" + "code" == "leetcode".
         *
         * s = "applepenapple", wordDict = { "apple", "pen" }
         * 返回 true, 因为 "apple" + "pen" + "apple" == s,
         * 注意：wordDict 中的每个 word 都可以被反复使用，只是至少有 1 个被使用至少 1 次。
         *
         * s = "catsandog", wordDict = { "meow", "cat", "and" }
         * 返回 false, 因为无论拿出 wordDict 中的多少个 word, 无论怎么拼接组合都得不到 s.
         */

        struct TestCase {
            std::string s;
            std::vector<std::string> wordDict;
            bool expected;
        };

        std::vector<TestCase> getTestCases() {
            return {
              TestCase {
                  .s = "leetcode",
                  .wordDict = { "leet", "code" },
                  .expected = true
              },
              TestCase {
                  .s = "applepenapple",
                  .wordDict = { "apple", "pen" },
                  .expected = true
              },
              TestCase {
                  .s = "catsandog",
                  .wordDict = { "meow", "cat", "and" },
                  .expected = false
              },
              TestCase {
                  .s = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
                  .wordDict = {"a","aa","aaa","aaaa","aaaaa","aaaaaa","aaaaaaa","aaaaaaaa","aaaaaaaaa","aaaaaaaaaa"},
                  .expected = false
              }
            };
        }

        class Solution {
        public:
            bool wordBreak(std::string s, std::vector<std::string>& wordDict) {

                // 该索引用来快速判断一个给定长的 word 是否在 wordDict 中
                std::unordered_map<size_t, std::unordered_set<std::string>> wordIndex;
                for (const auto &word : wordDict) {
                    wordIndex[word.size()].insert(word);
                }

                // canSuffixConstruct[i] 记录 s 的 i 长后缀可否由 wordDict 中的 word 构建
                size_t N = s.size();
                std::vector<uint8_t> canSuffixConstruct (N+1, 0);
                canSuffixConstruct[0] = 1;

                for (size_t i = 1; i <= N; ++i) {
                    for (size_t len = i; len >= 1; --len) {
                        if (wordIndex[len].count(s.substr(N-i, len)) && canSuffixConstruct[i-len]) {
                            canSuffixConstruct[i] = 1;
                            break;
                        }
                    }
                }

                return canSuffixConstruct[N] == 1;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_WORDBREAK_HPP
