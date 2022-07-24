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
            explicit KMPStringMatcher(std::string pattern) : _pattern(std::move(pattern)) {
                std::unordered_set<char> patternChars (_pattern.begin(), _pattern.end());

                if (_pattern.empty()) {
                    return;
                }

                dfa.resize(_pattern.size());
                dfa[0][_pattern[0]] = 1;

                for (size_t prevState = 0, currentState = 1; currentState < _pattern.size(); ++currentState) {
                    for (const auto &c : patternChars) {
                        dfa[currentState][c] = dfa[prevState][c];
                    }

                    dfa[currentState][_pattern[currentState]] = currentState+1;
                    prevState = dfa[prevState][_pattern[currentState]];
                }
            }

            intptr_t search(const std::string &text) {
                size_t patternSize = _pattern.size();
                size_t textSize = text.size();
                for (size_t i = 0, state = 0; i < textSize; ++i) {
                    state = dfa[state][text[i]];
                    if (state == patternSize) {
                        return i+1-patternSize;
                    }
                }

                return -1;
            }
        private:
            std::string _pattern;
            std::vector<std::unordered_map<char, size_t>> dfa;
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP
