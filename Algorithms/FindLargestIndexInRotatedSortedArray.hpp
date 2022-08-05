//
// Created by 韦晓枫 on 2022/8/5.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_FINDLARGESTINDEXINROTATEDSORTEDARRAY_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_FINDLARGESTINDEXINROTATEDSORTEDARRAY_HPP

#include <optional>
#include <vector>

namespace Algorithm::FindLargestIndexInRotatedSortedArray {

    struct Interval {
        size_t begin;
        size_t length;
    };

    struct TestCase {
        std::vector<int> input;
        std::optional<size_t> expectedOut;
    };

    std::vector<TestCase> getTestCases() {
        return {
            TestCase {
                .input = std::vector<int> { 1 },
                .expectedOut = {}
            },
            TestCase {
                .input = std::vector<int> { 1, 2 },
                .expectedOut = {}
            },
            TestCase {
                .input = std::vector<int> { 2, 1 },
                .expectedOut = 0
            },
            TestCase {
                .input = std::vector<int> { 1, 2, 3 },
                .expectedOut = {}
            },
            TestCase {
                .input = { 2, 3, 1 },
                .expectedOut = 1
            },
            TestCase {
                .input = { 3, 1, 2 },
                .expectedOut = 0
            },
            TestCase {
                .input = { 1, 2, 3, 4 },
                .expectedOut = {}
            },
            TestCase {
                .input = { 2, 3, 4, 1 },
                .expectedOut = 2
            },
            TestCase {
                .input = { 3, 4, 1, 2 },
                .expectedOut = 1
            },
            TestCase {
                .input = { 4, 1, 2, 3 },
                .expectedOut = 0
            }
        };
    }

    template <typename ElementT>
    bool isNormalInterval(const Interval &interval, const std::vector<ElementT> &ary, size_t totalLength) {
        size_t left = interval.begin % totalLength;
        size_t right = (left + interval.length - 1) % totalLength;
        return ary[left] < ary[right];
    }

    template <typename ElementT>
    std::optional<size_t> doFindSingularPoint(
        const Interval &interval,
        const std::vector<ElementT> &ary,
        size_t totalLength
    ) {
        if (ary.empty())
            return {};

        const size_t &N = totalLength;
        if (interval.length % 2 == 1) {
            if (interval.length <= 1) {
                return {};
            }

            size_t halfLength = (interval.length - 1) / 2;

            Interval left {
                .begin = interval.begin,
                .length = halfLength + 1
            };
            if (!isNormalInterval(left, ary, totalLength)) {
                return doFindSingularPoint(left, ary, totalLength);
            }

            Interval right {
                .begin = left.begin + halfLength,
                .length = halfLength + 1
            };
            if (!isNormalInterval(right, ary, totalLength)) {
                return doFindSingularPoint(right, ary, totalLength);
            }

            return {};
        } else {
            if (interval.length == 2) {
                if (isNormalInterval(interval, ary, totalLength))
                    return {};
                return interval.begin;
            }

            size_t half = interval.length / 2;
            Interval left {
                .begin = interval.begin,
                .length = half+1
            };
            if (!isNormalInterval(left, ary, totalLength)) {
                return doFindSingularPoint(left, ary, totalLength);
            }

            Interval right {
                .begin = (interval.begin + half - 1)%N,
                .length = half+1
            };
            if (!isNormalInterval(right, ary, totalLength)) {
                return doFindSingularPoint(right, ary, totalLength);
            }

            return {};
        }
    }

    template <typename ElementT>
    std::optional<size_t> findSingularPoint(const std::vector<ElementT> &ary) {
        size_t N = ary.size();
        Interval initialInterval {
            .begin = 0,
            .length = N
        };
        return doFindSingularPoint(initialInterval, ary, N);
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_FINDLARGESTINDEXINROTATEDSORTEDARRAY_HPP
