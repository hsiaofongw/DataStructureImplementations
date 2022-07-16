//
// Created by 韦晓枫 on 2022/7/16.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_FIRSTANDLASTPOSITIONOFTARGET_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_FIRSTANDLASTPOSITIONOFTARGET_HPP

#include <cstdint>
#include <vector>

namespace Algorithm {
    namespace FirstAndLastPositionOfTarget {
        /**
         * 找起、止下标：
         *
         * 问题描述：在一个已排序的，单调不减的数组中寻找一个给定元素的最小下标和最大下标。
         *
         * 思路：使用二分查找法，在找到元素后，不直接退出，继续向左寻找可以更新 minIdx,
         * 继续向右寻找可以更新 maxIdx.
         */

        using std::vector;

        enum class ContinueAt {
            CONTINUE_AT_LEFT,
            CONTINUE_AT_RIGHT
        };

        class Solution {
        public:
            vector<int> searchRange(vector<int>& nums, int target) {
                intptr_t minIdx = -1;
                intptr_t maxIdx = -1;
                doSearchRange(nums, target, 0, nums.size(), minIdx, maxIdx, ContinueAt::CONTINUE_AT_LEFT);
                doSearchRange(nums, target, 0, nums.size(), minIdx, maxIdx, ContinueAt::CONTINUE_AT_RIGHT);
                return { static_cast<int>(minIdx), static_cast<int>(maxIdx) };
            }

        private:
            void doSearchRange(
                    const vector<int> &nums,
                    int target,
                    intptr_t start,
                    intptr_t len,
                    intptr_t &minIdx,
                    intptr_t &maxIdx,
                    ContinueAt continueAt
            ) {
                if (len <= 0) {
                    return;
                }

                intptr_t mid, halfLen, rightHalfLen;
                if (len % 2 == 1) {
                    halfLen = (len-1)/2;
                    rightHalfLen = halfLen;
                    mid = start + halfLen;
                } else {
                    halfLen = len/2;
                    rightHalfLen = halfLen-1;
                    mid = start + halfLen;
                }

                if (target < nums[mid]) {
                    doSearchRange(nums, target, start, halfLen, minIdx, maxIdx, continueAt);
                } else if (target > nums[mid]) {
                    doSearchRange(nums, target, mid+1, rightHalfLen, minIdx, maxIdx, continueAt);
                } else {

                    if (continueAt == ContinueAt::CONTINUE_AT_LEFT) {
                        minIdx = mid;
                        doSearchRange(nums, target, start, halfLen, minIdx, maxIdx, continueAt);
                    } else if (continueAt == ContinueAt::CONTINUE_AT_RIGHT) {
                        maxIdx = mid;
                        doSearchRange(nums, target, mid+1, rightHalfLen, minIdx, maxIdx, continueAt);
                    } else {
                        // noop
                    }
                }
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_FIRSTANDLASTPOSITIONOFTARGET_HPP
