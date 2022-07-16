//
// Created by 韦晓枫 on 2022/7/16.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_LONGESTCONSECUTIVESEQUENCE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_LONGESTCONSECUTIVESEQUENCE_HPP

#include <unordered_map>
#include <vector>
#include <iostream>
#include <utility>
#include <memory>

namespace Algorithm {
    namespace LongestConsecutiveSequence {

        /**
         * 求最长连续自增子序列长度：
         *
         * 思路：
         * 1) 用 hashtable 把每个 num 映为一个指向其所属 consecutive 子序列描述对象的指针，
         * 并且该类对象的类型用一个带 min, max 字段的 struct 来定义即可；
         *
         * 2) 遍历原数组的唯一元素，遍历过程中连接首尾相接的 consecutive 子序列；
         */

        using std::vector;

        std::vector<std::pair<std::vector<int>, int>> getTestCases() {
            return {
                { {100,4,200,1,3,2}, 4 },    // 1 -> 2 -> 3 -> 4
                {{0,3,7,2,5,8,4,6,0,1}, 9}    // 0 -> 1 -> 2 -> ... -> 8
            };
        };

        class Solution {
        public:
            static int longestConsecutive(const vector<int>& nums) {

                struct Link {
                    int min;
                    int max;
                };

                std::unordered_map<int, std::shared_ptr<Link>> indexMap;
                for (const auto &num : nums) {
                    indexMap[num] = std::make_shared<Link>();
                    indexMap[num]->min = num;
                    indexMap[num]->max = num;
                }

                auto mergeTwoNum = [&indexMap](int current, int linkTo) -> void {
                    if (
                            indexMap.count(current) &&
                            indexMap.count(linkTo) &&
                            indexMap[current] != indexMap[linkTo]
                    ) {
                        int left, right, targetLeft, targetRight;
                        targetLeft = indexMap[linkTo]->min;
                        targetRight = indexMap[linkTo]->max;
                        if (indexMap[linkTo]->min < indexMap[current]->min) {
                            right = indexMap[current]->max;
                            left = indexMap[linkTo]->min;

                        } else if (indexMap[linkTo]->min > indexMap[current]->min) {
                            left = indexMap[current]->min;
                            right = indexMap[linkTo]->max;
                        } else {
                            assert((false));
                        }

                        indexMap[targetLeft] = indexMap[current];
                        indexMap[targetRight] = indexMap[current];
                        indexMap[current]->min = left;
                        indexMap[current]->max = right;
                    }
                };

                for (const auto &pair : indexMap) {
                    int current = pair.first;
                    mergeTwoNum(current, current+1);
                }

                int maxConsecutivesCount = 0;
                for (const auto &x : indexMap) {
                    maxConsecutivesCount = std::max(maxConsecutivesCount, x.second->max-x.second->min+1);
                }

                return maxConsecutivesCount;
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LONGESTCONSECUTIVESEQUENCE_HPP
