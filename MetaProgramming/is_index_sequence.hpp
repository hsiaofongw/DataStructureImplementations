//
// Created by 韦晓枫 on 2022/10/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_IS_INDEX_SEQUENCE_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_IS_INDEX_SEQUENCE_HPP

#include <type_traits>
#include <utility>

namespace MetaProgramming::IsIndexSequence {
    template <typename T, typename U, size_t... I>
    struct is_index_sequence_impl : std::false_type {};

    template <size_t... I>
    struct is_index_sequence_impl<std::integer_sequence<size_t, I...>, size_t, I...> : std::true_type {};

    template <typename T, size_t... I>
    struct is_index_sequence : is_index_sequence_impl<std::integer_sequence<size_t, I...>, size_t , I...> {};
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_IS_INDEX_SEQUENCE_HPP
