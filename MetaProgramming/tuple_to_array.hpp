//
// Created by 韦晓枫 on 2022/10/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_TUPLE_TO_ARRAY_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_TUPLE_TO_ARRAY_HPP

#include <type_traits>
#include <array>
#include <tuple>

namespace MetaProgramming::TupleToArray {

    namespace Impl1 {
        template <typename T, typename... Ts, std::size_t... I>
        consteval std::array<T, 1 + sizeof...(Ts)> tuple_to_array_impl(const std::tuple<T, Ts...>& t, std::index_sequence<I...>)
        {
            return { std::get<I>(t)... };
        }

        template <typename T, typename... Ts, typename Indices = std::make_index_sequence<1+sizeof...(Ts)>>
        consteval auto tuple_to_array(const std::tuple<T, Ts...>& t)
        {
            return tuple_to_array_impl(t, Indices {});
        }
    }

    namespace Impl2 {
        template <size_t N, typename T, typename... Ts> requires(N > 0)
        struct tuple_to_array_transformer : tuple_to_array_transformer<N - 1, T, T, Ts...> { };

        template <typename T, typename... Ts>
        struct tuple_to_array_transformer<1, T, Ts...> {
            constexpr static auto make_array(T a1, Ts... vs) {
                return std::array<T, 1 + sizeof...(Ts)> { a1, vs... };
            }
        };

        struct transform_tuple_to_array {
            template <typename T, typename... Ts, size_t N = 1 + sizeof...(Ts)>
            constexpr static auto transform(const std::tuple<T, Ts...> &t) {
                return std::apply(tuple_to_array_transformer<N, T>::make_array, t);
            }
        };
    }

}

#endif //DATASTRUCTUREIMPLEMENTATIONS_TUPLE_TO_ARRAY_HPP
