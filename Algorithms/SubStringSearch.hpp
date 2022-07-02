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

namespace Algorithm {
    namespace SubStringSearch {

        // Refer to STL templates
        using std::unique_ptr;
        using std::make_unique;
        using std::string;
        using std::set;
        using std::map;
        using std::vector;
        using std::empty;

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
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SUBSTRINGSEARCH_HPP
