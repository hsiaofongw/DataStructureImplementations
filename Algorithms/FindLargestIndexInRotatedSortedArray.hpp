//
// Created by 韦晓枫 on 2022/8/5.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_FINDLARGESTINDEXINROTATEDSORTEDARRAY_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_FINDLARGESTINDEXINROTATEDSORTEDARRAY_HPP

#include <optional>
#include <vector>

namespace Algorithm::RotatedArray {

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
            },
            TestCase {
                .input = { 1, 3, 5 },
                .expectedOut = {}
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

    template <typename ElementT>
    std::optional<size_t> doBisectSearchInRotatedSortedArray(
        const Interval &interval,
        const std::vector<ElementT> &ary,
        const ElementT &target,
        size_t totalLength
    ) {
        if (interval.length == 0) {
            return {};
        }

        if (interval.length % 2 == 1) {
            size_t halfLen = (interval.length-1) / 2;
            size_t mid = (interval.begin + halfLen) % totalLength;
            if (target < ary[mid]) {
                Interval left {
                    .begin = interval.begin,
                    .length = halfLen
                };
                return doBisectSearchInRotatedSortedArray(left, ary, target, totalLength);
            } else if (target > ary[mid]) {
                Interval right {
                    .begin = (mid + 1) % totalLength,
                    .length = halfLen
                };
                return doBisectSearchInRotatedSortedArray(right, ary, target, totalLength);
            } else {
                return mid;
            }
        } else {
            size_t halfLen = interval.length/2;
            size_t midRight = (interval.begin + halfLen) % totalLength;
            if (target < ary[midRight]) {
                Interval left {
                    .begin = interval.begin,
                    .length = halfLen
                };
                return doBisectSearchInRotatedSortedArray(left, ary, target, totalLength);
            } else if (target > ary[midRight]) {
                Interval right {
                    .begin = (midRight+1)%totalLength,
                    .length = halfLen-1
                };
                return doBisectSearchInRotatedSortedArray(right, ary, target, totalLength);
            } else {
                return midRight;
            }
        }
    }

    template <typename ElementT>
    std::optional<size_t> bisectSearchInRotatedSortedArray(
        const std::vector<ElementT> &ary,
        const ElementT &target
    ) {
        if (ary.empty())
            return {};

        size_t N = ary.size();
        std::optional<size_t> singularPoint = findSingularPoint(ary);
        if (singularPoint) {
            size_t maxIdx = singularPoint.value();
            size_t begin = (maxIdx+1) % N;
            Interval startInterval {
                .begin = begin,
                .length = N
            };
            return doBisectSearchInRotatedSortedArray(startInterval, ary, target, N);
        }

        Interval startInterval {
            .begin = 0,
            .length = N
        };
        return doBisectSearchInRotatedSortedArray(startInterval, ary, target, N);
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_FINDLARGESTINDEXINROTATEDSORTEDARRAY_HPP
