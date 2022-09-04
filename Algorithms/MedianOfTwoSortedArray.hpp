//
// Created by 韦晓枫 on 2022/9/4.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOSORTEDARRAY_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOSORTEDARRAY_HPP

#include <memory>
#include <vector>
#include <iostream>

namespace Algorithm::MedianOfTwoSortedArray {
    template <typename T>
    struct Slice {
        std::vector<T> *ptrV;
        size_t offset;
        size_t length;
    };

    size_t halfLen(size_t len) {
        if (len%2 == 1) {
            return (len+1)/2;
        } else {
            return len/2;
        }
    }

    template <typename T>
    T medianOfSlice(const Slice<T> &slice) {
        if (slice.length % 2 == 1) {
            return slice.ptrV->at(slice.offset + halfLen(slice.length) - 1);
        } else {
            size_t rightMid = slice.offset + halfLen(slice.length);
            return (slice.ptrV->at(rightMid-1) + slice.ptrV->at(rightMid)) / 2;
        }
    }

    template <typename T>
    T medianOfTwoSortedArraySlice(std::shared_ptr<Slice<T>> low, std::shared_ptr<Slice<T>> high) {
        while (true) {
            size_t lengthDecrease = std::min(halfLen(low->length), halfLen(high->length));

            low->length -= lengthDecrease;
            high->length -= lengthDecrease;

            if (low->length == 0 && high->length == 0) {
                return (low->ptrV->at(low->offset) + high->ptrV->at(high->offset)) / 2;
            }
            else if (low->length == 0) {
                return medianOfSlice(*high);
            }
            else if (high->length == 0) {
                return medianOfSlice(*low);
            } else {
                low->offset += lengthDecrease;
                if (medianOfSlice(*low) > medianOfSlice(*high)) {
                    low.swap(high);
                }
            }
        }
    }

    template <typename T>
    T medianOfTwoSortedArray(const std::vector<T> &a, const std::vector<T> &b) {
        Slice<T> sliceA { .ptrV = &a, .length = a.size(), .offset = 0 };
        Slice<T> sliceB { .ptrV = &b, .length = b.size(), .offset = 0 };

        std::shared_ptr<Slice<T>> ptrA (&sliceA);
        std::shared_ptr<Slice<T>> ptrB (&sliceB);

        if (medianOfSlice(sliceA) > medianOfSlice(sliceB)) {
            ptrA.swap(ptrB);
        }

        return medianOfTwoSortedArraySlice(ptrA, ptrB);
    }

    void generateTestCode() {
        for (size_t leftSize = 0; leftSize <= 4; leftSize++) {
            for (size_t rightSize = 0; rightSize <= 4; rightSize++) {

            }
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MEDIANOFTWOSORTEDARRAY_HPP
