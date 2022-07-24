//
// Created by 韦晓枫 on 2022/7/23.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_COINCHANGE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_COINCHANGE_HPP

#include <vector>
#include <algorithm>
#include <unordered_map>

namespace Algorithm {
    namespace CoinChange {
        /**
         * 换硬币问题：
         *
         * 有一个 uint32_t amount, 满足 0 <= amount <= UINT_MAX,
         * 以及一个数组 uint32_t coins[N], 1 <= N <= UINT_MAX,
         *
         * 试找到这样一组数 n1, n2, ..., n_N 满足
         * n1*coins[0] + n2*coins[1] + ... + n_N*coins[N] == amount (1),
         * 并且使得这组数的和 S = n1+n2+...+n_N 最大。
         *
         * 如果存在这样的一组数，返回 S, 如果不存在这样的一组数满足式 (1), 返回 -1.
         */

        struct TestCase {
            std::vector<int> coins;
            int amount;
            int expectedOutput;
        };

        std::vector<TestCase> getTestCases() {
            return {
                TestCase {
                    .coins = { 1,2,5 },
                    .amount = 11,
                    .expectedOutput = 3
                },
                TestCase {
                    .coins = { 2 },
                    .amount = 3,
                    .expectedOutput = -1
                },
                TestCase {
                    .coins = { 1 },
                    .amount = 0,
                    .expectedOutput = 0
                },
                TestCase {
                    .coins = {411,412,413,414,415,416,417,418,419,420,421,422},
                    .amount = 9864,
                    .expectedOutput = 24
                }
            };
        }

        class Solution {
        public:
            int coinChange(std::vector<int>& coins, int amount) {
                assert((!coins.empty()));
                note.clear();
                std::sort(coins.begin(), coins.end());
                return coinChangeWithCache(coins, amount, coins.size());
            }

        private:

            std::unordered_map<int, std::unordered_map<size_t, int>> note;

            int coinChangeWithCache(const std::vector<int> &coins, int amount, size_t prefix) {
                if (note.count(amount) && note[amount].count(prefix)) {
                    return note[amount][prefix];
                }

                int ans = doCoinChange(coins, amount, prefix);
                note[amount][prefix] = ans;
                return ans;
            }

            int doCoinChange(const std::vector<int> &coins, int amount, size_t prefix) {
                if (prefix == 1) {
                    if (amount % coins[0] != 0) {
                        return -1;
                    }

                    return amount / coins[0];
                }

                int largestCoin = coins[prefix-1];
                if (amount < largestCoin) {
                    return coinChangeWithCache(coins, amount, prefix-1);
                }

                int choice1 = coinChangeWithCache(coins, amount-largestCoin, prefix);
                int choice2 = coinChangeWithCache(coins, amount, prefix-1);

                if (choice1 == -1 && choice2 == -1) {
                    return -1;
                } else if (choice1 == -1) {
                    return choice2;
                } else if (choice2 == -1) {
                    return 1 + choice1;
                } else {
                    return std::min(1+choice1, choice2);
                }
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_COINCHANGE_HPP
