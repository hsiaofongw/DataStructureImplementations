//
// Created by 韦晓枫 on 2022/7/21.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_QUICKSORT_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_QUICKSORT_HPP

#include <vector>
#include <functional>
#include "../Interfaces/TestCase.hpp"
#include "../Utils/RandomInteger.h"

namespace Algorithm {
    namespace Sorting {

        template <typename T>
        using Comparator = std::function<bool (const T &a, const T &b)>;
        using SignedInteger = int32_t;
        using SignedVector = std::vector<SignedInteger>;
        using TestCase = Interface::TestCase<SignedVector, SignedVector>;

        std::vector<TestCase> getTestCases() {
            // 让一个 std::initializer<TestCase> 被隐式地转为 std::vector<TestCase>
            return {
                {
                    .question = SignedVector { 67, 56, 32, 11, 199, 280, 2828 },
                    .expectedOutput = SignedVector { 11, 32, 56, 67, 199, 280, 2828 }
                },
                {
                    .question = SignedVector { },
                    .expectedOutput = SignedVector { }
                },
                {
                    .question = SignedVector { 1 },
                    .expectedOutput = SignedVector { 1 }
                },
                {
                    .question = SignedVector { 1, 2 },
                    .expectedOutput = SignedVector { 1, 2 }
                },
                {
                    .question = SignedVector { 2, 1 },
                    .expectedOutput = SignedVector { 1, 2 }
                },
                {
                    .question = SignedVector { 2, 1, 3 },
                    .expectedOutput = SignedVector { 1, 2, 3 }
                },
                {
                    .question = SignedVector { 3, 1, 2 },
                    .expectedOutput = SignedVector { 1, 2, 3 }
                },
                {
                    .question = SignedVector { 3, 2, 1 },
                    .expectedOutput = SignedVector { 1, 2, 3 }
                },
                {
                    .question = SignedVector { 4,1,2,3 },
                    .expectedOutput = SignedVector { 1,2,3,4 }
                },
                {
                    .question = SignedVector { 3,1,4,2 },
                    .expectedOutput = SignedVector { 1,2,3,4 }
                },
                {
                    .question = SignedVector { 1,2,3,4 },
                    .expectedOutput = SignedVector { 1,2,3,4 }
                },
                {
                    .question = SignedVector { 4,1,3,8,5 },
                    .expectedOutput = SignedVector { 1,3,4,5,8 }
                },
                {
                    .question = SignedVector { 11, 12, 13, 14, 1 },
                    .expectedOutput = SignedVector { 1, 11, 12, 13, 14 }
                }
            };
        }

        template<typename T>
        void exchange(std::vector<T> &ary, size_t i, size_t j) {
            T temp = ary[i];
            ary[i] = ary[j];
            ary[j] = temp;
        }

        template <typename T>
        void quickSort(std::vector<T> &ary, intptr_t begin, intptr_t N, Comparator<T> &isLessThanOrEqual) {

            // 若 N <= 1, 则无需对比。
            if (N <= 1)
                return;

            // 随机选定 pivot 元素下标
            Utils::RandomIntegerGenerator<size_t> pivotIdxGen (begin, begin+N-1);
            size_t pivotIdx = pivotIdxGen.get();

            exchange(ary, begin, pivotIdx);
            size_t leftCnt = 0;
            for (size_t i = 0; i < N-1; ++i) {
                size_t elementIdx = begin+1+i;
                if (isLessThanOrEqual(ary[elementIdx], ary[begin])) {
                    ++leftCnt;
                    exchange(ary, elementIdx, begin+leftCnt);
                }
            }

            // 此时，leftCnt 表示 ary[begin+1..begin+N] 范围内小于等于 pivot 的元素的个数
            // 再加上 pivot 本身
            ++leftCnt;

            // 现在 leftCnt 表示 ary[begin..begin+N] 范围内小于等于 pivot 的元素的个数
            // 它的值，随着输入的不同，出现的范围是在 [1, N].

            // 把轴元素换回正确位置
            // 使得轴元素左侧（如果有）都不超过它，右侧都大于它
            exchange(ary, begin, begin+leftCnt-1);

            // 继续对轴元素左边 leftCnt - 1 个元素进行排序
            quickSort(ary, begin, leftCnt - 1, isLessThanOrEqual);

            // 继续对轴元素右边 N - leftCnt 个元素排序
            quickSort(ary, begin+leftCnt, N-leftCnt, isLessThanOrEqual);
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_QUICKSORT_HPP
