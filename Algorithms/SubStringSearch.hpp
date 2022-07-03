//
// Created by 韦晓枫 on 2022/7/3.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP

#include <memory>
#include <string>
#include <set>
#include <map>
#include <cctype>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <iostream>

namespace Algorithm {
    namespace SubStringSearch {

        // Refer to STL templates
        using std::unique_ptr;
        using std::shared_ptr;
        using std::make_unique;
        using std::string;
        using std::set;
        using std::map;
        using std::vector;
        using std::empty;
        using std::initializer_list;
        using std::cout;

        // Alias DFATable typename
        using DFATable = vector<map<char8_t, size_t>>;

        // Implement how to compute the DFA table in KMP algorithm
        unique_ptr<DFATable> dfaBuilder(const string &pattern) {
            set<char8_t> patternChars {};
            for (const auto& patternChar : pattern) {
                patternChars.insert(patternChar);
            }

            size_t M = pattern.size();
            auto dfaPtr = make_unique<DFATable>(M);
            if (empty(pattern)) {
                return dfaPtr;
            }
            DFATable &dfa = *dfaPtr;

            // When at initial state (a.k.a. state 0),
            // if it got a pattern[0], then it moves to state 1
            dfa[0][pattern[0]] = 1;

            // So, in DFA table, row index represent where we already are,
            // more specifically,
            // i in dfa[i][c] means "We are in such a situation that seeking pattern[i]".
            // c in dfa[i][c] is a char, let y = dfa[i][c]
            // these means:
            // "As we are in a situation that looks for pattern[i],
            // when we got a char like c, we would move to state y."

            for (size_t colIdx = 1, prevState = 0; colIdx < M; ++colIdx) {
                for (const auto &patternChar : patternChars) {
                    dfa[colIdx][patternChar] = dfa[prevState][patternChar];
                }
                dfa[colIdx][pattern[colIdx]] = colIdx + 1;
                prevState = dfa[prevState][pattern[colIdx]];
            }

            return dfaPtr;
        }

        struct TestCase {
            TestCase(initializer_list<string> initLst) {
                auto initLstIt = initLst.begin();
                this->text = *initLstIt; // 第一个参数用来初始化 this->text
                ++initLstIt;

                if (initLstIt != initLst.end()) {
                    this->pattern = *initLstIt; // 第二个参数用来初始化 this->pattern
                }
            }

            string text;
            string pattern;
        };

        vector<TestCase> getTestCases() {
            return vector<TestCase> {
                    { "ababbbbab", "babb" },
                    { "aaaaabbbabababab", "abbbbb" },
                    { "aaaaabbbabababab", "aabbb" },
                    { "AABRAACADABRAACAADABRA", "AACAA" },
                    { "AABRAACADABRAACAADABRA", "ABABAC" }
            };
        }

        class KMPStringMatcher {
        public:

            /**
             * 一个 KMPStringMatcher 对象在构造之时就需要知道 Pattern 的具体内容，
             * 从而依据 Pattern 的内容完成对 DFA 的构建。
             */
            explicit KMPStringMatcher(const shared_ptr<string>& _patternPtr) :
                patternPtr(_patternPtr),
                dfaPtr(dfaBuilder(*_patternPtr))
            { }

            /**
             * 在文本 text: string 中搜索子串 pattern: string,
             * 若这样的 pattern 子串在 text 中出现了，则返回 pattern 在 text 中的下标，
             * 若这样的 pattern 子串在 text 中出现了多次，则返回的那个下标指向 pattern 最先出现的那个位置，
             * 若这样的 pattern 子串在 text 中没有出现过，则返回 text 作为一个 std::basic_string<char> 的长度。
             */
            size_t search(const string &text) {
                const string &pattern = *this->patternPtr;
                DFATable &dfa = *this->dfaPtr;
                size_t textLen = text.size(), patternLen = pattern.size();
                size_t textOffset = 0;
                for (
                    size_t patternOffset = 0;
                    textOffset < textLen && patternOffset < patternLen;
                    ++textOffset
                ) {
                    char8_t inputChar = text[textOffset];
                    size_t nextState = dfa[patternOffset][inputChar];
                    patternOffset = nextState;

                    if (patternOffset == patternLen) {
                        return textOffset-patternLen+1;
                    }
                }

                return textLen;
            }

            /** （调试用）打印 DFA 表项 */
            void debugPrintDFATable() {
                DFATable &dfa = *this->dfaPtr;
                for (size_t rowIdx = 0; rowIdx < dfa.size(); ++rowIdx) {
                    const auto &row = dfa[rowIdx];
                    for (const auto &pair : row) {
                        cout << "("
                            << rowIdx << ", "
                            << static_cast<char>(pair.first) << ", "
                            << pair.second
                            << ")\n";
                    }
                }
            }

        private:
            unique_ptr<DFATable> dfaPtr;
            shared_ptr<string> patternPtr;
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP
