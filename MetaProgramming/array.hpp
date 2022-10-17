//
// Created by 韦晓枫 on 2022/10/17.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_ARRAY_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_ARRAY_HPP
#include <iostream>
#include <array>

namespace MetaProgramming::Array {

    template <typename T>
    struct showType;

    template <typename T, size_t N, size_t M, size_t... aIndices, size_t... bIndices>
    constexpr std::array<T, N+M> concat_array(std::array<T, N> a, std::array<T, M> b, std::integer_sequence<size_t, aIndices...>, std::integer_sequence<size_t, bIndices...>) {
        return std::array<T, N+M> { a[aIndices]..., b[bIndices]... };
    }

    template <typename T, size_t N, size_t M>
    constexpr std::array<T, N+M> concat_array(std::array<T, N> a, std::array<T, M> b) {
        return concat_array(a, b, std::make_integer_sequence<size_t, N> {}, std::make_integer_sequence<size_t, M> {});
    }

    template <typename T, size_t N>
    constexpr auto concat_arrays(std::array<T, N> a) {
        return a;
    }

    template <typename T, size_t n1, size_t n2, size_t... ns, typename... Ts>
    constexpr auto concat_arrays(std::array<T, n1> a1, std::array<T, n2> a2,  std::array<Ts, ns>... restArrays) {
        return concat_arrays(concat_array(a1, a2), restArrays...);
    }

    template <typename T>
    constexpr std::array<T, 2> make_array(T x, T y) {
        return { x, y };
    }

    template <typename T, typename... Ts>
    constexpr std::array<std::array<T, 2>, sizeof...(Ts)> make_arrays(T x, Ts... y) {
        return { make_array(x, y)... };
    }

    template <int N, int M, typename...>
    struct cartesian_product : cartesian_product<M, N, std::make_integer_sequence<int, N>, std::make_integer_sequence<int, M>> {};

    template <int N, int M, int... lhsIndices, int... rhsIndices>
    struct cartesian_product<N, M, std::integer_sequence<int, lhsIndices...>, std::integer_sequence<int, rhsIndices...>> {
        constexpr static auto value = concat_arrays(make_arrays(lhsIndices, rhsIndices...)...);
    };

    template <typename T, typename...>
    struct zero_array_impl {};

    template <int N>
    struct zero_array_impl<std::array<int, N>> : zero_array_impl<std::array<int, N>, std::make_integer_sequence<int, N>> {};

    template <int N, int... indices>
    struct zero_array_impl<std::array<int, N>, std::integer_sequence<int, indices...>> {
        constexpr static std::array<int, N> value { (indices, 0)... };
    };

    template <typename T, int N>
    struct zero_array : zero_array_impl<std::array<T, N>> {};

//    int main() {
//        auto x = cartesian_product<3, 5>::value;
//
//        constexpr std::array<int, 3> a = {11,12,13};
//        constexpr std::array<int, 5> b = {4,5,6,7,8};
//        constexpr auto c = concat_array(a, b);
//
//        showType<std::integral_constant<std::array<int, 8>, c>> ();
//        showType<std::integral_constant<std::array<int, 5>, concat_arrays(std::array<int, 1> {91}, std::array<int, 2> {21, 22}, std::array<int, 2> {5,3})>> ();
//        showType<std::integral_constant<std::array<std::array<int, 2>, 15>, cartesian_product<3, 5>::value>> ();
//
//        for (const auto &ele : c) {
//            std::cout << ele << std::endl;
//        }
//
//        std::cout << "Hello, World!" << std::endl;
//        return 0;
//    }

}

#endif //DATASTRUCTUREIMPLEMENTATIONS_ARRAY_HPP
