//
// Created by 韦晓枫 on 2022/7/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_WORDBREAK_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_WORDBREAK_HPP

#include <string>
#include <vector>
#include <unordered_map>

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
              }
            };
        }

        class Solution {

            using WordIndex = std::unordered_map<char, std::vector<size_t>>;

        public:
            bool wordBreak(std::string s, std::vector<std::string>& wordDict) {
                WordIndex wordIndex;
                size_t wordCnt = wordDict.size();
                for (size_t idx = 0; idx < wordCnt; ++idx) {
                    if (!wordDict[idx].empty()) {
                        wordIndex[wordDict[idx][0]].push_back(idx);
                    }
                }

                return wordBreak(s, 0, wordDict, wordIndex);
            }

        private:
            bool wordBreak(
                const std::string &s,
                size_t offset,
                const std::vector<std::string> &wordDict,
                WordIndex &wordIndex
            ) {
                if (offset >= s.size()) {
                    return true;
                }

                for (const auto &wordIdx : wordIndex[s[offset]]) {
                    const std::string &word = wordDict[wordIdx];
                    if (word.size() <= s.size()-offset && word == s.substr(offset, word.size())) {
                        if (wordBreak(s, offset+word.size(), wordDict, wordIndex)) {
                            return true;
                        }
                    }
                }

                return false;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_WORDBREAK_HPP
