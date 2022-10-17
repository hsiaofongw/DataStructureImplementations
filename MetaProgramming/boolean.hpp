//
// Created by 韦晓枫 on 2022/10/17.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_BOOLEAN_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_BOOLEAN_HPP

#include <type_traits>
#include <array>

namespace MetaProgramming::Boolean {
    template <typename T>
    struct some_true {};

    template <bool... vs>
    struct some_true<std::integer_sequence<bool, vs...>> : std::integral_constant<bool, (vs || ... || false)> {};

    template <typename... Ts>
    bool some_true_func(Ts... vs) {
        return (vs || ... || false);
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_BOOLEAN_HPP
