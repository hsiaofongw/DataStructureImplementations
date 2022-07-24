//
// Created by 韦晓枫 on 2022/7/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SUBSETS_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SUBSETS_HPP

#include <vector>
#include <bitset>
#include <cmath>

namespace Algorithm {
    namespace Subsets {

        struct TestCase {
            std::vector<int> nums;
            std::vector<std::vector<int>> subsets;
        };

        std::vector<TestCase> getTestCases() {
            return {
              TestCase {.nums = {1,2,3},.subsets={
                      {}, {1}, {2}, {3},
                      {1,2},{1,3},{2,3},
                      {1,2,3}
              }},
              TestCase {.nums = {}, .subsets={{}}},
              TestCase {.nums = {1}, .subsets={
                      {},{1}
              }}
            };
        }

        /**
         * 子集合生成算法：
         *
         * 定义（子集）：设 A, B 是集合，若 A 集合不包含不属于 B 的元素，则集合 A 称为集合 B 的子集。
         *
         * 子集关系是集合范畴上的一个偏序关系，它满足：
         *
         * 1. 传递性：若 A 是 B 的子集，B 是 C 的子集，那么 A 是 B 的子集；
         * 2. 自反性：任何集合 X 都是其自身的子集；
         */
        template<typename ElementType, size_t MaximumElementCount = 64>
        std::vector<std::vector<ElementType>> subsets(const std::vector<ElementType> &a) {
            size_t N = a.size();
            assert((N <= MaximumElementCount));

            std::vector<std::vector<ElementType>> subsetStore;
            long double N_double = N;
            size_t numOfSubsets = powl(2.0, N_double);
            for (size_t subsetIdx = 0; subsetIdx < numOfSubsets; ++subsetIdx) {
                std::bitset<MaximumElementCount> selectionBit(subsetIdx);
                std::vector<ElementType> subset;
                for (size_t elementIdx = 0; elementIdx < N; ++elementIdx) {
                    if (selectionBit[elementIdx]) {
                        subset.push_back(a[elementIdx]);
                    }
                }
                subsetStore.template emplace_back(subset);
            }

            return subsetStore;
        }

        class Solution {
        public:
            std::vector<std::vector<int>> subsets(std::vector<int> &nums) {
                 return Algorithm::Subsets::subsets<int>(nums);
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SUBSETS_HPP
