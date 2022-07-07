//
// Created by 韦晓枫 on 2022/7/7.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_PERMUTATIONS_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_PERMUTATIONS_HPP


#include <iostream>
#include <bitset>
#include <vector>
#include <list>
#include <cmath>
#include <memory>

namespace Algorithm {

    namespace Permutation {

        void doGeneratePermutations(
                std::list<size_t> &letters,
                std::vector<size_t> &permutation,
                std::vector<std::vector<size_t>> &permutations,
                size_t level
        ) {
            if (letters.empty()) {
                if (!permutation.empty()) {
                    permutations.emplace_back(permutation);
                }
                return;
            }

            for (size_t i = 0; i < letters.size(); ++i) {
                size_t headIdx = letters.front();
                letters.pop_front();
                permutation[level] = headIdx;
                doGeneratePermutations(letters, permutation, permutations, level + 1);
                letters.push_back(headIdx);
            }
        }

        std::unique_ptr<std::vector<std::vector<size_t>>> getPermutations(size_t n) {
            std::list<size_t> letters;
            for (size_t i = 0; i < n; ++i) {
                letters.push_back(i);
            }

            std::vector<size_t> permutationBuffer(n);
            size_t permutationsCount = 1;
            for (size_t i = 0; i < n; ++i) {
                permutationsCount *= i + 1;
            }
            auto permutationsPtr = std::make_unique<std::vector<std::vector<size_t>>>();
            permutationsPtr->reserve(permutationsCount);

            doGeneratePermutations(letters, permutationBuffer, *permutationsPtr, 0);
            return permutationsPtr;
        }
    }

}


#endif //DATASTRUCTUREIMPLEMENTATIONS_PERMUTATIONS_HPP
