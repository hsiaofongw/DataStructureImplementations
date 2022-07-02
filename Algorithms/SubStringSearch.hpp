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
            dfa[0][pattern[0]] = 1;
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
