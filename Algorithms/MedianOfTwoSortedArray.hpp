//
// Created by 韦晓枫 on 2022/9/4.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOSORTEDARRAY_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOSORTEDARRAY_HPP

#include <memory>
#include <vector>
#include <iostream>
#include <array>

namespace Algorithm::MedianOfTwoSortedArray {

    template <typename T>
    struct Slice {
        T* data;
        size_t offset;
        size_t length;
    };

    template <typename T>
    double medianOfSlice(const Slice<T> &slice) {
        if (slice.length % 2 == 1) {
            return slice.data[slice.offset + (slice.length-1) / 2];
        } else {
            size_t halfLen = slice.length / 2;
            return (slice.data[slice.offset + halfLen - 1] + slice.data[slice.offset + halfLen]) / 2.0;
        }
    }

    // 返回在 ary 中所有严格比 element 小的元素的个数。
    template <typename T>
    size_t rankFloor(const T &element, const Slice<T> &ary) {
        size_t len = ary.length;
        size_t offset = ary.offset;

        while (true) {
            if (len == 0) {
                // 对于空数组 ary，element 插入之后 index 一定是 0
                return offset - ary.offset;
            }

            bool isOdd = len % 2 == 1;
            size_t half;
            double median;
            if (isOdd) {
                half = (len-1) / 2;
                median = ary.data[offset + half];
            } else {
                half = len / 2;
                median = (ary.data[offset + half-1] + ary.data[offset + half]) / 2.0;
            }

            if (element > median) {
                offset += half;
                if (isOdd) {
                    offset += 1;
                }
            }
            len = half;
        }
    }

    template <typename T>
    double medianOfTwoSortedArraySlice(std::shared_ptr<Slice<T>> low, std::shared_ptr<Slice<T>> high) {
        while (true) {
            if (low->length > high->length)
                low.swap(high);

            if (low->length <= 2) {
                std::array<size_t, 2> insertIndices { 0, 0 };
                for (size_t i = 0; i < low->length; i++)
                    insertIndices[i] = rankFloor(low->data[low->offset+i], *high);

                size_t M = low->length + high->length;
                std::array<size_t, 2> targetIndices { 0, 0 };
                size_t targetIndicesCnt = 0;
                if (M%2 == 1) {
                    targetIndices[targetIndicesCnt++] = (M-1) / 2;
                } else {
                    targetIndices[targetIndicesCnt++] = M/2 - 1;
                    targetIndices[targetIndicesCnt++] = M/2;
                }

                std::array<double, 2> medians {0.0, 0.0};
                size_t medianCnt = 0;
                for (size_t i = 0; i < targetIndicesCnt; i++) {
                    size_t targetIdx = targetIndices[i];
                    bool found = false;
                    for (int j = low->length-1; j >= 0; j--) {
                        size_t insertIdx = insertIndices[j];
                        if (insertIdx == targetIdx) {
                            medians[medianCnt++] = low->data[low->offset + j];
                            found = true;
                            break;
                        }
                        if (targetIdx > insertIdx)
                            targetIdx--;
                    }

                    if (!found)
                        medians[medianCnt++] = high->data[high->offset + targetIdx];
                }

                return (medians[0] + medians[1]) / ((double) medianCnt);
            }

            if (medianOfSlice(*low) > medianOfSlice(*high))
                low.swap(high);

            size_t deltaLen;

            deltaLen = (low->length % 2 == 1) ? (low->length - 1) / 2 : low->length / 2 - 1;
            low->offset += deltaLen;
            low->length -= deltaLen;

            high->length -= (high->length % 2 == 1) ? (high->length - 1) / 2 : high->length / 2 - 1;;
        }
    }

    template <typename T>
    double medianOfTwoSortedArray(std::vector<T> &a, std::vector<T> &b) {
        auto ptrA = std::make_shared<Slice<T>>();
        auto ptrB = std::make_shared<Slice<T>>();
        ptrA->data = a.data();
        ptrA->length = a.size();
        ptrA->offset = 0;
        ptrB->data = b.data();
        ptrB->length = b.size();
        ptrB->offset = 0;

        return medianOfTwoSortedArraySlice(ptrA, ptrB);
    }


}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOSORTEDARRAY_HPP
