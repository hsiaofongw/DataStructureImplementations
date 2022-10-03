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

namespace Algorithm::MiniMax {
    // 玩家选边站队情况描述枚举类型，红方总是希望分数越大越好，而蓝方做为红方的抬杠者，总是希望分数越小越好，
    // 简单来说，当分数为 +Infinity 时，可以认为红方胜利，当分数为 -Infinity 时，可以认为蓝方胜利。
    enum class PlayerColor { RED, BLUE, TBD };

    // 玩家信息描述符
    struct Player {
        Player( ) : color(PlayerColor::TBD) { }

        explicit Player(PlayerColor color) : color(color) { }

        // 所属战队
        PlayerColor color;

        // 直接对手
        std::shared_ptr<Player> opponent;
    };

    class Board {
        using BoardBitMap = std::unordered_map<uint8_t, std::unordered_map<uint8_t, PlayerColor>>;
        using FreeSlots = std::unordered_map<uint8_t, std::unordered_set<uint8_t>>;
        using MoveDescriptor = std::tuple<uint8_t, uint8_t, PlayerColor>;

    private:
         BoardBitMap occupied;
         FreeSlots freeSlots;
         std::stack<MoveDescriptor> history;
         static constexpr uint8_t nRow = 3;
         static constexpr uint8_t nCol = 3;
         std::unordered_map<PlayerColor, std::shared_ptr<Player>> players;

    public:
        Board( ) {
            auto redPlayer = std::make_shared<Player>(PlayerColor::RED);
            auto bluePlayer = std::make_shared<Player>(PlayerColor::BLUE);
            auto nature = std::make_shared<Player>(PlayerColor::TBD);
            redPlayer->opponent = bluePlayer;
            bluePlayer->opponent = redPlayer;
            players[PlayerColor::TBD] = nature;
            players[PlayerColor::RED] = redPlayer;
            players[PlayerColor::BLUE] = bluePlayer;
        }

        ~Board() {
            for (auto &pair_ : players) {
                if (auto player = pair_.second) {
                    if (player->color != PlayerColor::TBD) {
                        player->opponent = nullptr;
                        break;
                    }
                }
            }
        }

        // 走子
        void move(uint8_t i, uint8_t j, PlayerColor color) {
            freeSlots[i].erase(j);
            occupied[i][j] = color;
            history.push(std::tuple (i, j, color));
        }

        // 悔棋
        void undo() {
            if (!history.empty()) {
                auto moveDescriptor = history.top();
                history.pop();
                uint i = std::get<0>(moveDescriptor);
                uint j = std::get<1>(moveDescriptor);
                freeSlots[i].insert(j);
                occupied[i].erase(j);
            }
        }

        // 判赢
        bool hasWin(PlayerColor color) {
            using PlayerView = std::bitset<nCol*nRow>;
            PlayerView playerView;
            for (auto &pair_1 : occupied) {
                uint8_t i = pair_1.first;
                for (auto &pair_2 : pair_1.second) {
                    uint8_t j = pair_2.first;
                    PlayerColor color_ = pair_2.second;
                    if (color_ == color) {
                        uint8_t idx = i * nCol + j;
                        playerView.set(idx);
                    }
                }
            }

            constexpr static uint16_t xBase = 0b111;
            constexpr static uint16_t yBase = 0b1001001;
            constexpr static uint64_t z1Base = 0b100010001;
            constexpr static uint64_t z2Base = 0b1010100;

            constexpr static PlayerView x (xBase);
            constexpr static PlayerView x1 (xBase << 3);
            constexpr static PlayerView x2 (xBase << 6);
            constexpr static PlayerView y (yBase);
            constexpr static PlayerView y1 (yBase << 1);
            constexpr static PlayerView y2 (yBase << 2);
            constexpr static PlayerView z1 (z1Base);
            constexpr static PlayerView z2 (z2Base);

            constexpr static PlayerView winEigenVectors[] = { x, x1, x2, y, y1, y2, z1, z2 };

            return std::any_of(
                std::begin(winEigenVectors),
                std::end(winEigenVectors),
                [&playerView](const auto &eigenVector) -> bool {
                    return eigenVector == (eigenVector & playerView);
                }
            );
        }

        // 从棋盘的左往右、从上往下，寻找第一个空位
        std::optional<std::tuple<uint8_t, uint8_t>> findFirstFreeSlot() {
            std::vector<uint8_t> freePosIndices;
            for (auto &pair_ : freeSlots) {
                uint8_t i = pair_.first;
                for (uint8_t j : pair_.second) {
                    freePosIndices.push_back(i * nCol + j);
                }
            }

            if (std::empty(freePosIndices)) {
                return {};
            }

            std::sort(freePosIndices.begin(), freePosIndices.end(), std::less<uint8_t> {});
            uint8_t idx = freePosIndices[0];
            uint8_t i = idx / nCol;
            uint8_t j = idx % nCol;
            return std::tuple ( i, j );
        }

        // 获取最近一次走子记录
        std::optional<MoveDescriptor> getLastMove() {
            if (history.empty()) {
                return {};
            }

            return history.top();
        }

        // 获取对手阵营信息
        PlayerColor getOpponentColor(PlayerColor color) {
            if (auto player = players[color]) {
                return player->color;
            }
            return PlayerColor::TBD;
        }
    };

    // MiniMax Algorithm (with Alpha-Beta pruning out of box)





    template <typename T, typename = void>
    struct has_begin_end : std::false_type {};

    template <typename T>
    struct has_begin_end<T, std::void_t<typename T::begin, typename T::end>> : std::true_type {};

    struct S {
        using IntVector = std::vector<int>;
        decltype(std::declval<IntVector>().begin()) begin;
        decltype(std::declval<IntVector>().end()) end;
    };

    template <typename T, class U>
    double minimax(
        const T &boardConfiguration,
        const Player &currentPlayer,
        std::function<std::enable_if_t<has_begin_end<U>::value, U&> (const T&, const Player&)> getPossibleMoves,
        std::function<double (const T&)> staticEvaluate
    ) {
        // 初始最佳分数 -Infinity（负无穷大）
        double bestScore = 0 - std::numeric_limits<double>::infinity();
        // 初始最差分数 Infinity（正无穷大）
        double worstScore = std::numeric_limits<double>::infinity();

        auto &moves = getPossibleMoves(boardConfiguration, currentPlayer);
        auto beginIt = std::begin(moves);
        auto endIt = std::end(moves);
        std::for_each(beginIt, endIt, [](const auto &element) -> void {

        });

        if (currentPlayer.color == PlayerColor::RED) {
            // 评估当前棋局对于红方选手的效用分数：
            //
            // 估分逻辑是这样的：当前，红方已经走了，并且棋局状态是 boardConfiguration, 要评估这样的局面对于红方而言效用分数是多少，
            // 我们考虑蓝方在接下来的每一种可能走子，我们简单地假定蓝方一定会走对于红方最不利（也就是分最低）的走子，基于这个假设，
            // 当前局面对于红方的效用分数，就等于接下来蓝方所能作出的对红方最不利的走子导致的局面的得分。

        } else if (currentPlayer.color == PlayerColor::BLUE) {
            // 评估当前棋局对于蓝方选手的效用分数：
            //
            // 逻辑和评估红方得分的略有类似：我们可以假定接下来红方会走对自己最有利（从而对蓝方最不利）的走子，
            // 基于这个假定预测红方接下来会怎么走，然后拿这个预测来算分。
        } else {
            std::cerr << "选手战队未确定，无法评估。" << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MINIMAX_HPP
