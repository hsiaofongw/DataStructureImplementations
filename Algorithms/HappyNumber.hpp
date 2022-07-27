//
// Created by 韦晓枫 on 2022/7/27.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_HAPPYNUMBER_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_HAPPYNUMBER_HPP

#include <unordered_set>
#include <vector>

namespace Algorithm::HappyNumber {



    /**
     * Happy Number 判定过程：
     *
     * 定义 (happy number):
     * 一个 happy number 是这样的一个 int n,
     * 它的 10 进制形式的 digits 记做 d[n,1], d[n,2], ..., d[n,N(n)]（N 是它的最大位数），
     * 若数列
     *   a[0] = n, a[i] = d[a[i-1],1]^2 + ... + d[a[i-1],N(a[i-1])], i = 1,2,3,...
     * 收敛到 1, 则称 n 为一个 happy number.
     *
     * 问题描述：
     * 任给一个 int n, 试判定 n 是否为一个 happy number.
     *
     * 解题思路：
     * 1. 你可以用一个 Set 来判定数列是否出现了循环；
     * 2. 若数列的某一项的值是 1, 则不难推出：该项后续也都是 1, 这样的数列收敛的一个充分（不必要）条件是：它的某一项是 1;
     * 3. 你要实现一个子例程，该子例程将 n 拆解为 10 进制 digits（非必需）；
     * 4. 若从数列的某一项开始出现了重复（出现了之前出现过了的值），则该数列发散（后续项的值都是在一个序列上循环）；
     */
    class Solution {
    public:
        bool isHappy(int n) {
            std::unordered_set<int> appearedNums;
            while (true) {
                std::vector<int> digits (11, 0); // int 的 10 进制形式最大不超过 11 位数
                integerToDigits(n, digits);
                n = 0;
                for (const auto &digit : digits)
                    n += digit*digit;
                if (appearedNums.count(n))
                    return n == 1;
                appearedNums.insert(n);
            }
        }
    private:
        void integerToDigits(int x, std::vector<int> &digits) {
            digits[10] = 0; // int 10 进制形式的第 11 位永远是 0
            constexpr size_t bitsCount = 10;
            constexpr int powOf10[bitsCount] = {
                    1, // 10^0, 对应个位
                    10, // 10^1, 对应十位
                    100, // 10^2, 对应百位
                    1000, // 10^3, 对应千位
                    10000, // 10^4, 对应万位
                    100000, // 10^5, 对应十万位
                    1000000, // 10^6, 对应百万位
                    10000000, // 10^7, 对应千万位
                    100000000, // 10^8, 对应亿位
                    1000000000, // 10^9, 对应十亿位（int 最大不超过 50 亿, 也就四十多亿）
            };
            int bitValue = 0;
            for (size_t i = 0; i < bitsCount; ++i) {
                size_t bitIdx = bitsCount-1-i;
                bitValue = x / powOf10[bitIdx];
                digits[bitIdx] = bitValue;
                x -= bitValue * powOf10[bitIdx];
            }
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_HAPPYNUMBER_HPP
