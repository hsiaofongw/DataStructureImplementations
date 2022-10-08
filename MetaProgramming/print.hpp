//
// Created by 韦晓枫 on 2022/10/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_PRINT_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_PRINT_HPP

#include <iostream>
#include <tuple>

namespace MetaProgramming::Print {
    template <typename T, size_t N, typename U, size_t Idx = 0>
    void traverse_array(const U &func, const std::array<T, N> &ary) {
        if constexpr (Idx < N) {
            func(ary[Idx]);
            traverse_array<T, N, U, Idx+1>(func, ary);
        }
    }

    template <typename U, size_t Idx = 0, typename... Ts>
    void traverse_tuple(const U &func, const std::tuple<Ts...> &t) {
        if constexpr (Idx < sizeof...(Ts)) {
            func(std::get<Idx>(t));
            traverse_tuple<U, Idx+1, Ts...>(func, t);
        }
    }

    template <typename... Ts>
    void print_tuple(const std::tuple<Ts...> &t) {
        std::cout << "Tuple:" << std::endl;
        size_t tupleIdx = 0;
        traverse_tuple(
            [&tupleIdx](const auto &x) -> void {
                std::cout << '[' << tupleIdx++ << "]: " << x << std::endl;
            },
            t
        );
    }

    template <typename T, size_t N>
    void print_array(const std::array<T, N> &a) {
        std::cout << "Array, size: " << N << std::endl;
        size_t lineIdx = 0;
        traverse_array(
            [&lineIdx](const auto &x) -> void {
                std::cout << '[' << lineIdx++ << "]: " << x << std::endl;
            },
            a
        );
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_PRINT_HPP
