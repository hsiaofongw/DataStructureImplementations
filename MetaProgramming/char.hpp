//
// Created by 韦晓枫 on 2022/10/17.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_CHAR_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_CHAR_HPP

#include <type_traits>
#include <array>

namespace MetaProgramming::Char {
    template <typename T, int repeats, typename...>
    struct repeat_char_impl {};

    template <char c, int repeats>
    struct repeat_char_impl<std::integral_constant<char, c>, repeats> : repeat_char_impl<
        std::integral_constant<char, c>,
        repeats,
        std::make_integer_sequence<size_t, repeats>
    > {};

    template <char c, int repeats, size_t... indices>
    struct repeat_char_impl<std::integral_constant<char, c>, repeats, std::integer_sequence<size_t, indices...>> {
        constexpr static char value[repeats+1] = { (indices, c)..., 0 };
    };

    template <char c, int repeats>
    struct repeat_char : repeat_char_impl<std::integral_constant<char, c>, repeats> {};
}


#endif //DATASTRUCTUREIMPLEMENTATIONS_CHAR_HPP
