//
// Created by 韦晓枫 on 2022/7/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_FIBONACCI_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_FIBONACCI_HPP

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <iostream>

namespace Algorithm {
    namespace Fibonacci {
        /**
         * 求斐波那契数列第 N 项
         *
         * 公式：
         * F(1) = 1, F(2) = 1, F(n) = F(n-1) + F(n-2), n >= 3.
         */

        class Solution {
        public:
            uint64_t getNth(uint32_t n) {
                assert((n >= 1));

                if (n <= this->maxN) {
                    // std::cout << "Cache hit.\n";
                    return this->fibonacci[n-1];
                }

                // std::cout << "Cache miss.\n";

                this->fibonacci.resize(n);
                for (auto i = this->maxN; i < n; ++i) {
                    this->fibonacci[i] = this->fibonacci[i-1] + this->fibonacci[i-2];
                }
                this->maxN = n;

                return this->fibonacci[n-1];
            }

        private:
            uint32_t maxN = 2;
            std::vector<uint64_t> fibonacci = { 1, 1 };
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_FIBONACCI_HPP
