//
// Created by 韦晓枫 on 2022/6/25.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_LONGESTPALINDROMESUBSTRING_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_LONGESTPALINDROMESUBSTRING_HPP

#include <string>
#include <vector>
#include <iostream>

namespace Algorithm {
    namespace LongestPalindromeSubString {
        using std::string;

        std::vector<string> getTestCases() {
            return std::vector<std::string> {
                    "babad", "cbbd", "abccbc", "aeiou", "aacabdkacaa"
            };
        }

        class Solution {
        public:
            static string longestPalindrome(string s) {
                ptrdiff_t maxWindowSize = 0;
                ptrdiff_t maxWindowOffset = 0;

                auto updateLongestPalindrome = [&maxWindowSize, &maxWindowOffset](ptrdiff_t newWindowOffset, ptrdiff_t newWindowSize) -> void {
                    maxWindowSize = newWindowSize;
                    maxWindowOffset = newWindowOffset;
                };

                auto windowSliding = [&s, &updateLongestPalindrome, &maxWindowSize](ptrdiff_t rightWingStartOffset) -> void {
                    const ptrdiff_t len = s.size(); // todo: 或许后续可以考虑用 intptr_t ?
                    for (ptrdiff_t offset = 0; offset < len; ++offset) {
                        ptrdiff_t wing = 0;
                        while (true) {
                            ptrdiff_t left = offset - wing;
                            ptrdiff_t right = offset + rightWingStartOffset + wing;
                            if (left >= 0 && right < len) {
                                auto leftChar = s[left];
                                auto rightChar = s[right];
                                ptrdiff_t windowSize = right - left + 1;
                                if (leftChar == rightChar) {
                                    if (windowSize > maxWindowSize) {
                                        updateLongestPalindrome(left, windowSize);
                                    }
                                    ++wing;
                                    continue;
                                }
                            }
                            break;
                        }
                    }
                };

                // 偶滑窗
                windowSliding(1);

                // 奇滑窗
                windowSliding(0);

                // 奇偶滑窗的顺序无所谓

                if (maxWindowSize == 0) {
                    return "";
                } else {
                    return string { s.substr(maxWindowOffset, maxWindowSize) };
                }
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_LONGESTPALINDROMESUBSTRING_HPP
