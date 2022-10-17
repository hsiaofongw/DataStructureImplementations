#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <type_traits>
#include <vector>
#include <array>

enum class Color { Red, Blue };
using Move = std::tuple<size_t, size_t, Color>;

/** 获取对手颜色 */
auto getOpponentColor(Color color) -> Color {
    if (color == Color::Blue) {
        return Color::Red;
    } else {
        return Color::Blue;
    }
}

template <typename T>
concept BoardType =
    std::is_integral_v<decltype(T::total_size)> &&
    requires(T t, std::size_t n) {
        { t.data[n] } -> std::same_as<typename T::reference>;
    };

template <typename T>
concept IntegralValueTypeIterator =
    std::input_iterator<T> &&
    std::integral<typename std::iterator_traits<T>::value_type>;

template <typename T>
concept IntegralElementContainer =
    IntegralValueTypeIterator<decltype(std::declval<T>().begin())> &&
    requires(T t) {
        { std::begin(t) } -> std::same_as<decltype(std::end(t))>;
    };

template <BoardType BoardT, size_t total_size = BoardT::total_size, IntegralElementContainer IntsType>
double minimax(
    BoardT&& board,
    const std::function<IntsType &&(BoardT&&)> &getMoves,
    const std::function<double (BoardT&&)> &staticEvaluation
) {
    std::cout << "total size: " << total_size << std::endl;
    IntsType&& moves = getMoves(board);
    auto it = std::begin(moves);
    auto movesEnd = std::end(moves);
    if (it == movesEnd) {
        return staticEvaluation(board);
    }

    return 0;
}

struct Board {
    using value_type = Color;
    using reference = value_type&;
    using difference_type = int;
    constexpr static size_t total_size = 9;
    std::array<Color, total_size> data;
};

template <typename T>
class show_type;

int main() {
    minimax(Board {});

    Board b;
//    show_type<decltype(std::declval<Board>().data[0])>();


//    testBoard(1);
//    Foo1 foo1;
//    testBoard(foo1);

//    Foo2 foo2;
//    testBoard(foo2);

//    Foo3 foo3;
//    testBoard(foo3);

//    Foo4 foo4;
//    testBoard(foo4);

    return 0;
}
