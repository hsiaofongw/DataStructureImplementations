//
// Created by 韦晓枫 on 2022/9/24.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MINIMAX_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MINIMAX_HPP

#include <memory>
#include <functional>
#include <limits>
#include <vector>
#include <array>
#include <iostream>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <optional>
#include <algorithm>
#include <tuple>

namespace Algorithm::MiniMax {
    // 玩家选边站队情况描述枚举类型，红方总是希望分数越大越好，而蓝方做为红方的抬杠者，总是希望分数越小越好，
    // 简单来说，当分数为 +Infinity 时，可以认为红方胜利，当分数为 -Infinity 时，可以认为蓝方胜利。
    enum class PlayerColor { RED, BLUE, TBD };

    namespace WinEigenVectorsImplDetail {
        constexpr  uint16_t xBase = 0b111;
        constexpr  uint16_t yBase = 0b1001001;
        constexpr  uint64_t z1Base = 0b100010001;
        constexpr  uint64_t z2Base = 0b1010100;

        constexpr uint16_t x (xBase);
        constexpr uint16_t x1 (xBase << 3);
        constexpr uint16_t x2 (xBase << 6);
        constexpr uint16_t y (yBase);
        constexpr uint16_t y1 (yBase << 1);
        constexpr uint16_t y2 (yBase << 2);
        constexpr uint16_t z1 (z1Base);
        constexpr uint16_t z2 (z2Base);

        constexpr uint16_t winEigenVectors[] = { x, x1, x2, y, y1, y2, z1, z2 };

        /**
         * 对 base 对最右边 baseLen 位重复 repeatCount 遍，例如：
         * repeatCount = 4, base = 0b101, baseLen = 3,
         * 则输出为：0b101_101_101_101
         */
        template <size_t repeatCount, uint64_t base, size_t baseLen>
        struct repeats : std::integral_constant<
            uint64_t,
            ((base << (baseLen * (repeatCount - 1))) | repeats<repeatCount-1, base, baseLen>::value)
        > {};

        template <uint64_t base, size_t baseLen>
        struct repeats<1, base, baseLen> : std::integral_constant<uint64_t, base> {};

        template <uint64_t base, size_t baseLen>
        struct repeats<0, base, baseLen> : std::integral_constant<uint64_t, 0> {};

        /** 输出最右边含 N 个 1 的二进制整数，例如 N = 6, 则输出 0b111_111 */
        template <size_t N>
        struct ones : std::integral_constant<uint64_t, ((~((uint64_t)0)) >> (64-N))> {};

        template <size_t NRows, size_t NCols>
        struct win_eigenvector_rows {
            constexpr static auto value = std::tuple_cat(
                std::tuple<uint64_t>(ones<NCols>::value << ( (NRows-1) * NCols ) ),
                win_eigenvector_rows<NRows-1, NCols>::value
            );
        };

        template <size_t NCols>
        struct win_eigenvector_rows<1, NCols> {
            constexpr static std::tuple<uint64_t> value = std::tuple<uint64_t>(ones<NCols>::value);
        };

        template <size_t NRows, size_t NCols, size_t ColIdx = NCols>
        struct win_eigenvector_cols {
            constexpr static auto value = std::tuple_cat(
                std::tuple<uint64_t>(repeats<NRows, (uint64_t) 1, NCols>::value << (ColIdx - 1)),
                win_eigenvector_cols<NRows, NCols, ColIdx-1>::value
            );
        };

        template <size_t NRows, size_t NCols>
        struct win_eigenvector_cols<NRows, NCols, 1> {
            constexpr static auto value = std::tuple<uint64_t>(repeats<NRows, (uint64_t) 1, NCols>::value);
        };

        template <size_t NRows, size_t NCols>
        struct win_eigenvectors {};

        template <int i_, int j_>
        struct point_ {
            constexpr static int i = i_;
            constexpr static int j = j_;
        };

        /** i-j==lineIdx 在 [0,maxI]x[0,maxJ] 范围内的起始点，做为迭代的起点 */
        template <int lineIdx>
        struct negative_diag_start_point : std::conditional_t<lineIdx >= 0, point_<lineIdx, 0>, point_<0, 0-lineIdx>> {};

        /** 寻找由参数 LineIdx 确定的直线 { (i,j) : i-j == LineIdx } 在 [0,maxI]x[0,maxJ] 区域内的所有整数点 */
        template <int lineIdx, int maxI, int maxJ, int i = negative_diag_start_point<lineIdx>::i, int j = negative_diag_start_point<lineIdx>::j>
        struct find_negative_diag_points {
            constexpr static auto value = std::tuple_cat(
                find_negative_diag_points<lineIdx, maxI, maxJ, i+1, j+1>::value,
                std::tuple(point_<i, j>{})
            );
        };

        template <int lineIdx, int maxI, int maxJ, int j>
        struct find_negative_diag_points<lineIdx, maxI, maxJ, maxI+1, j> {
            constexpr static auto value = std::tuple<>();  // empty tuple, intentional.
        };

        template <int lineIdx, int maxI, int maxJ, int i>
        struct find_negative_diag_points<lineIdx, maxI, maxJ, i, maxJ+1> {
            constexpr static auto value = std::tuple<>();  // empty tuple, intentional.
        };
    }

    using WinEigenVectorsImplDetail::winEigenVectors;

    // 玩家信息描述符
    struct Player {
        Player( ) : color(PlayerColor::TBD) { }

        explicit Player(PlayerColor color) : color(color) { }

        // 所属战队
        PlayerColor color;

        // 直接对手
        std::shared_ptr<Player> opponent;
    };

    template <typename T>
    concept Integral = std::is_integral_v<T> || std::is_enum_v<T>;

    template <size_t N, Integral CellT, CellT emptyMark>
    struct BoardData {
        BoardData(const BoardData<N, CellT, emptyMark> &rhs) : cells(rhs.cells) { }
        constexpr static CellT emptyMark_ = emptyMark;
        std::array<CellT, N> cells;
    };

    template <size_t N, Integral CellT, CellT emptyMark>
    class MoveIterator {
        using value_type = BoardData<N, CellT, emptyMark>;
        using reference = value_type&;
        using difference_type = std::ptrdiff_t;
        using self_type_ = MoveIterator<N, CellT, emptyMark>;

    private:
        size_t idx;
        std::unique_ptr<value_type> boardData_;
        CellT color;

        void moveToNext() {
            size_t prevIdx = idx;
            ++idx;
            while (idx < N) {
                if (boardData_[idx] == emptyMark) {
                    boardData_[prevIdx] = emptyMark;
                    boardData_[idx] = color;
                    break;
                }
                ++idx;
            }
        }

    public:
        MoveIterator(const value_type &boardData, size_t idx, CellT color) :
            color(color),
            idx(idx),
            boardData_(std::make_unique<value_type>(boardData))
        {
            if (idx < N) {
                boardData_[idx] = color;
            }
        }

        // prefix ++
        self_type_ &operator++() {
            moveToNext();
            return *this;
        }

        // suffix ++
        self_type_ operator++(int) {
            MoveIterator prev (*boardData_, idx, color);
            moveToNext();
            return prev;
        }

        reference getView() {
           return *boardData_;
        }

        reference operator*() {
            return getView();
        }

        bool operator==(self_type_ &rhs) {
            return idx == rhs.idx;
        }

        bool operator!=(self_type_ &rhs) {
            return idx != rhs.idx;
        }
    };

    template <typename T>
    concept Iterable = requires(T t) {
        std::input_iterator<decltype(t.begin())>;
        std::input_iterator<decltype(t.end())>;
    };

    template <typename T> requires std::input_iterator<T>
    struct MovesContainer {
        MovesContainer(T &&begin_, T &&end_) : begin_(std::move(begin_)), end_(std::move(end_)) {}

        T begin_;
        T end_;

        T &begin() { return begin_; }
        T &end() { return end_; }
    };

    template <size_t N>
    using PlayerView = std::bitset<N>;

    template <size_t N, Integral CellT, CellT emptyMark>
    PlayerView<N> getPlayerView(BoardData<N, CellT, emptyMark> &boardData, CellT color) {
        PlayerView<N> playerView;
        size_t cellsCount = std::size(boardData.cells);
        for (size_t i = 0; i < cellsCount; i++) {
            if (boardData.cells[i] == color) {
                playerView.set(i);
            }
        }

        return playerView;
    }

    template <size_t NRows, size_t NCols, Integral CellT, CellT emptyMark>
    bool win(BoardData<NRows*NCols, CellT, emptyMark> &boardData, CellT color) {
        for (const uint16_t &eigen : winEigenVectors) {
            std::bitset<NRows*NCols> eigen_ (eigen);
        }
    }

    template <size_t N, Integral CellT, CellT emptyMark>
    MovesContainer<MoveIterator<N, CellT, emptyMark>> getSubsequentMoves(
        BoardData<N, CellT, emptyMark> &boardData,
        CellT color,
        std::unordered_map<CellT, CellT> &opponentColor
    ) {
        auto findOpponent = opponentColor.find(color);
        CellT opponent = findOpponent == opponentColor.end() ? color : *findOpponent;
        if (opponent == color) {
            std::cerr << "Can't find opponent." << std::endl;
        }



    }

    template <typename ViewT, typename ColorT, Iterable T>
    double minimax(
        const ViewT &view,
        const ColorT &player,
        std::function<T (const ViewT& view)> getSubsequentMoves
    ) {
        T moves = getSubsequentMoves(view);
        auto begin = moves.begin();
        auto end = moves.end();
        for (; begin != end; ++begin) {

        }
    }

    template <std::input_iterator T>
    void test(T t) {
        ++t;
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MINIMAX_HPP
