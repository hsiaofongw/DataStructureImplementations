//
// Created by 韦晓枫 on 2022/10/8.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_GENERATE_SCAN_LINES_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_GENERATE_SCAN_LINES_HPP

#include <iostream>
#include <type_traits>
#include <tuple>
#include <array>
#include <iomanip>

namespace MetaProgramming::GenerateScanLines {
/**
 * 生成扫描线
 *
 * 扫描线是离散的点组成的集合，对于一个 nxn 的棋盘，我们可以定义下列扫描线（簇）：
 *
 * (1) { (i,j) : i == k }, k = 0,1,...,n-1;
 * (2) { (i,j) : j == k }, k = 0,1,...,n-1;
 * (3) { (i,j) : i-j = k }, k = -(n-1), -(n-1)+1, ..., 0, 1, 2, ..., n-1;
 * (4) { (i,j) : i+j = k }, k = (n-1), -(n-1)+1, ..., 0, 1, 2, ..., n-1;
 *
 * 其中 i,j 只能为整数且落在 [0,n-1]x[0,n-1] 范围内，k 为参数。我们还把 (3) 称为 negative diagonal 也即负对角线，
 * 把 (4) 称为 positive diagonal 也即正对角线。
 *
 * 当 n 足够小时，它（棋盘）的每一条扫描线的点集数据都可以编码进一个 uint64_t 对象中进行存储和计算，
 * 特别地，若将棋盘上一个种类的棋子的落子分布数据也编码为一个 uint64_t 对象，则在程序中就可以方便地判断玩家是否已经胜利。
 *
 * 举个例子：
 * 0b100010001 是 k=0 的负对角线，是 3x3 棋盘的一条扫描线，它对应棋盘的落子情况是：
 * |======|
 * |x     |
 * |  x   |
 * |    x |
 * ========
 * 换言之，它代表某个玩家取得从3x3棋盘左上到右下连续3个子连城一条线的格局，我们要在程序上判断玩家有没有达到这样的格局，
 * 我们只需从棋盘中把该玩家落下的子单独筛选出来，并且也编码为一个 uint64_t 对象进行判断，譬如说，现在棋盘是
 * |======|
 * |x 0 x |
 * |  x 0 |
 * |0 0 x |
 * ========
 * 对于 x 玩家，单独筛选出
 * |======|
 * |x   x |
 * |  x   |
 * |    x |
 * ========
 * 对于 0 玩家，单独筛选出
 * |======|
 * |  0   |
 * |    0 |
 * |0 0   |
 * ========
 * x 玩家的棋局编码是 0b101010001, 0 玩家的棋局编码是 0b010001110, 回顾刚才的 k=0 扫描线是 0b100010001,
 * 因为 0b100010001 (扫描线) & 0b101010001 (x玩家) == 0b100010001 (扫描线), 所以 x 玩家胜利,  ::式(1)
 * 又因为 0b100010001 (扫描线) & 0b010001110 (0玩家) != 0b100010001 (扫描线), 所以 0 玩家没有胜利。
 *
 * 这就是根据扫描线来判断玩家胜负的基本原理，事实上，只要 ::式(1) 对任何一条扫描线成立，就可宣判该玩家胜利。
 */


    template<int i_, int j_>
    struct point : std::type_identity<std::integer_sequence<int, i_, j_>> { };

    struct points { };

    template<typename T, typename U>
    struct points_cat : std::type_identity<std::tuple<points>> { };

    template<typename T, typename... Ts>
    struct points_cat<T, std::tuple<points, Ts...>> : std::type_identity<std::tuple<points, T, Ts...>> { };

    template<typename T, typename U, int nPoints>
    struct make_points : std::type_identity<std::tuple<points>> { };

    template<int i0, int j0, int di, int dj, int nPoints>
    struct make_points<point<i0, j0>, point<di, dj>, nPoints> : points_cat<
            point<i0, j0>,
            typename make_points<point<i0 + di, j0 + dj>, point<di, dj>, nPoints - 1>::type
    > { };

    template<int i0, int j0, int di, int dj>
    struct make_points<point<i0, j0>, point<di, dj>, 0> : std::type_identity<std::tuple<points>> { };

    template<int lineIdx>
    struct negative_diag_start : std::conditional<lineIdx >= 0, point<lineIdx, 0>, point<0, 0 - lineIdx>> { };

    template<typename T, T n>
    struct my_abs : std::integral_constant<T, (n >= 0 ? n : 0 - n)> { };

    template<int edge, int lineIdx>
    struct negative_diag : make_points<
            typename negative_diag_start<lineIdx>::type,
            point<1, 1>,
            edge + 1 - my_abs<int, lineIdx>::value
    > { };

    template<int lineIdx, int edge>
    struct positive_diag_start : std::conditional<
            lineIdx <= edge,
            point<0, lineIdx>,
            point<lineIdx - edge, edge>
    > { };

    template<int edge, int lineIdx>
    struct positive_diag_len : std::integral_constant<int, (lineIdx > edge ? edge + 1 - (lineIdx - edge) : lineIdx +
                                                                                                           1)> { };

    template<int edge, int lineIdx>
    struct positive_diag : make_points<
            typename positive_diag_start<lineIdx, edge>::type,
            point<1, -1>,
            positive_diag_len<edge, lineIdx>::value
    > { };

    template<int edge, int lineIdx>
    struct horizontal : make_points<point<lineIdx, 0>, point<0, 1>, edge + 1> { };

    template<int edge, int lineIdx>
    struct vertical : make_points<point<0, lineIdx>, point<1, 0>, edge + 1> { };

/**
 * line 是一个 tuple 对象，tuple 里面的每一个元素都是 point_ 类型的，
 * encode_line 的意思是说：把一个 line 的信息编码到一个 uint64_t 对象中进行存储。
 */

    template<int sideLen, uint64_t base, typename T>
    struct encode_point : std::integral_constant<uint64_t, base> { };

    template<int sideLen, uint64_t base, int i, int j>
    struct encode_point<sideLen, base, point<i, j>> : std::integral_constant<uint64_t, (base | (((uint64_t) 1)
            << (sideLen * sideLen - (i * sideLen + j) - 1)))> { };

    template<int edge, int base, typename T>
    struct encode_line_impl : std::integral_constant<uint64_t, base> { };

    template<int edge, int base>
    struct encode_line_impl<edge, base, std::tuple<points>> : std::integral_constant<uint64_t, base> { };

    template<int edge, int base, typename T, typename... Ts>
    struct encode_line_impl<edge, base, std::tuple<points, T, Ts...>> : encode_line_impl<edge, encode_point<
            edge + 1, base, T>::value, std::tuple<points, Ts...>> { };

    template<int edge, typename T>
    struct encode_line : encode_line_impl<edge, 0, T> { };

    template<typename T, typename U>
    struct sequence_cat : std::type_identity<std::integer_sequence<int>> { };

    template<typename T, T... nums, T n>
    struct sequence_cat<std::integral_constant<T, n>, std::integer_sequence<T, nums...>>
            : std::type_identity<std::integer_sequence<T, n, nums...>> { };

    template<int start, int step, int n>
    struct make_sequence : sequence_cat<
            std::integral_constant<int, start>,
            typename make_sequence<start + step, step, n - 1>::type
    > { };

    template<int start, int step>
    struct make_sequence<start, step, 0> : std::type_identity<std::integer_sequence<int>> { };

    template<int edge>
    struct horizontal_line_indices : make_sequence<0, 1, edge + 1> { };

    template<int edge>
    struct vertical_line_indices : make_sequence<0, 1, edge + 1> { };

    template<int edge>
    struct negative_diag_line_indices : make_sequence<0 - edge + 2, 1, edge * 2 - 3> { };

    template<int edge>
    struct positive_diag_line_indices : make_sequence<2, 1, edge * 2 - 3> { };

    template<typename T, T... nums>
    auto print_sequence(std::integer_sequence<T, nums...> s) {
        auto result = std::array<T, s.size()>{(std::cout << nums << ", ", nums)...};
        std::cout << std::endl;
        return result;
    }

    template<int i, int j>
    void print_point(point<i, j>...) {
        std::cout << "point<" << i << ", " << j << "> ";
    }

    void print_points(std::tuple<points>) {
        std::cout << std::endl;
    }

    template<typename T, typename... Ts>
    void print_points(std::tuple<points, T, Ts...>...) {
        print_point(T{ });
        print_points(std::tuple<points, Ts...>{ });
    }

    template<typename...>
    struct join_tuples : std::type_identity<std::tuple<>> { };

    template<typename... T, typename... U, typename... Rest>
    struct join_tuples<std::tuple<T...>, std::tuple<U...>, Rest...> : join_tuples<std::tuple<T..., U...>, Rest...> { };

    template<typename T>
    struct join_tuples<T> : std::type_identity<T> { };

    template<typename... Ts>
    struct concat : std::type_identity<std::tuple<>> { };

    template<typename T, typename... Ts>
    struct concat<std::tuple<Ts...>, T> : std::type_identity<std::tuple<Ts..., T>> { };

    template<int edge, typename T>
    struct encode_lines : concat<> { };

    template<int edge, typename T, typename... Ts>
    struct encode_lines<edge, std::tuple<T, Ts...>> : concat<
            typename encode_lines<edge, std::tuple<Ts...>>::type,
            std::integral_constant<uint64_t, encode_line<edge, T>::value>
    > { };

    template<int edge>
    struct encode_lines<edge, std::tuple<>> : concat<> { };

    template<int edge, typename T, template<int, int> typename U>
    struct generate_lines_from_indices : concat<> { };

    template<int edge, typename T, T v, T... vs, template<int, int> typename U>
    struct generate_lines_from_indices<edge, std::integer_sequence<T, v, vs...>, U> : concat<
            typename generate_lines_from_indices<edge, std::integer_sequence<T, vs...>, U>::type,
            typename U<edge, v>::type
    > { };

    template<int edge, typename T, template<int, int> typename U>
    struct generate_lines_from_indices<edge, std::integer_sequence<T>, U> : concat<> { };

    template<int edge>
    struct all_positive_diags : generate_lines_from_indices<
            edge,
            typename positive_diag_line_indices<edge>::type,
            positive_diag
    > { };

    template<int edge>
    struct all_negative_diags : generate_lines_from_indices<
            edge,
            typename negative_diag_line_indices<edge>::type,
            negative_diag
    > { };

    template<int edge>
    struct all_horizontal_lines : generate_lines_from_indices<
            edge,
            typename horizontal_line_indices<edge>::type,
            horizontal
    > { };

    template<int edge>
    struct all_vertical_lines : generate_lines_from_indices<
            edge,
            typename vertical_line_indices<edge>::type,
            vertical
    > { };

    void print_lines(std::tuple<>) {
        std::cout << std::endl;
    }

    template<typename T, typename... Ts>
    void print_lines(std::tuple<T, Ts...>) {
        print_points(T{ });
        print_lines(std::tuple<Ts...>{ });
    }

    void print_binary(uint64_t x) {
        std::bitset<64> r(x);
        std::cout << "encoded: " << r.to_string() << std::endl;
    }

    template<int edge, int i, int j>
    void print_encoded_point(point<i, j>) {
        std::cout << "point<" << i << ", " << j << "> ";
        print_binary(encode_point<edge + 1, 0, point<i, j>>::value);
    }

    template<int edge>
    void print_encoded_line(std::tuple<points>) { }

    template<int edge, typename T, typename... Ts>
    void print_encoded_line(std::tuple<points, T, Ts...>) {
        print_points(std::tuple<points, T, Ts...>{ });
        print_binary(encode_line<edge, std::tuple<points, T, Ts...>>::value);
    }

    template<int edge>
    void print_encoded_lines(std::tuple<>) { }

    template<int edge, typename T, typename... Ts>
    void print_encoded_lines(std::tuple<T, Ts...>) {
        print_encoded_line<edge>(T{ });
        print_encoded_lines<edge>(std::tuple<Ts...>{ });
    }

    void print_tuple_of_integrals_in_binary(std::tuple<>) { }

    template<typename T, T n, typename... Ts>
    void print_tuple_of_integrals_in_binary(std::tuple<std::integral_constant<T, n>, Ts...>) {
        std::bitset<sizeof(T) * 8> r(n);
        std::cout << std::setw(6) << n << ": " << r.to_string() << std::endl;
        print_tuple_of_integrals_in_binary(std::tuple<Ts...>{ });
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_GENERATE_SCAN_LINES_HPP
