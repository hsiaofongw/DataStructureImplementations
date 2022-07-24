//
// Created by 韦晓枫 on 2022/7/3.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP

#include <memory>
#include <string>
#include <unordered_set>
#include <cctype>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <unordered_map>

namespace Algorithm {
    namespace SubStringSearch {

        // Alias DFATable typename
        using DFATable = std::vector<std::unordered_map<char, size_t>>;

        struct TestCase {
            std::string text;
            std::string pattern;
            intptr_t expectedAt;
        };

        std::vector<TestCase> getTestCases() {
            return std::vector<TestCase> {
                TestCase { .text = "ababbbbab", .pattern = "babb", .expectedAt = 1 },
                TestCase { .text = "aaaaabbbabababab", .pattern = "abbbbb", .expectedAt = -1 },
                TestCase { .text = "aaaaabbbabababab", .pattern = "aabbb", .expectedAt = 3 },
                TestCase { .text = "AABRAACADABRAACAADABRA", .pattern = "AACAA", .expectedAt = 12 },
                TestCase { .text = "AABRAACADABRAACAADABRA", .pattern = "ABABAC", .expectedAt = -1 }
            };
        }

        class KMPStringMatcher {
        public:

            /**
             * 一个 KMPStringMatcher 对象在构造之时就需要知道 Pattern 的具体内容，
             * 从而依据 Pattern 的内容完成对 DFA 的构建。
             */
            explicit KMPStringMatcher(std::string _pattern) : pattern(std::move(_pattern)), dfa() {
                if (pattern.empty()) {
                    return;
                }

                // Implement how to compute the DFA table in KMP algorithm
                std::unordered_set<char> patternChars (pattern.begin(), pattern.end());

                size_t M = pattern.size();
                dfa.resize(M);

                // When at initial state (a.k.a. state 0),
                // if it got a pattern[0], then it moves to state 1
                dfa[0][pattern[0]] = 1;

                // So, in DFA table, row index represent where we already are,
                // Specifically:
                // i in dfa[i][c] means "We are in such a situation that seeking pattern[i]".
                // c in dfa[i][c] is a char, let y = dfa[i][c]
                // these means:
                // "As we are in a situation that looks for pattern[i],
                // when we got a char like c, we would move to state y."

                for (size_t colIdx = 1, prevState = 0; colIdx < M; ++colIdx) {
                    for (const auto &patternChar : patternChars) {
                        // If we do not get a char we want, but some char we got previously,
                        // then the dfa will let the state back to last time we get that char.
                        dfa[colIdx][patternChar] = dfa[prevState][patternChar];
                    }
                    dfa[colIdx][pattern[colIdx]] = colIdx + 1;
                    prevState = dfa[prevState][pattern[colIdx]];
                }
            }

            /**
             * 在文本 text: std::string 中搜索子串 pattern: std::string,
             * 若这样的 pattern 子串在 text 中出现了，则返回 pattern 在 text 中的下标，
             * 若这样的 pattern 子串在 text 中出现了多次，则返回的那个下标指向 pattern 最先出现的那个位置，
             * 若这样的 pattern 子串在 text 中没有出现过，则返回 -1.
             */
            intptr_t search(const std::string &text) {
                size_t textLen = text.size();
                size_t patternLen = pattern.size();
                size_t textOffset = 0;
                for (
                    size_t patternOffset = 0;
                    textOffset < textLen && patternOffset < patternLen;
                    ++textOffset
                ) {
                    char inputChar = text[textOffset];
                    size_t nextState = dfa[patternOffset][inputChar];
                    patternOffset = nextState;

                    if (patternOffset == patternLen) {
                        return textOffset-patternLen+1;
                    }
                }

                return -1;
            }

            /** （调试用）打印 DFA 表项 */
            void debugPrintDFATable() {
                for (size_t rowIdx = 0; rowIdx < dfa.size(); ++rowIdx) {
                    const auto &row = dfa[rowIdx];
                    for (const auto &pair : row) {
                        std::cout << "("
                            << rowIdx << ", "
                            << static_cast<char>(pair.first) << ", "
                            << pair.second
                            << ")\n";
                    }
                }
            }

        private:
            std::string pattern;
            DFATable dfa;
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP
