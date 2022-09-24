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

namespace Algorithm::MiniMax {
    // MiniMax Algorithm (with Alpha-Beta pruning out of box)

    // 玩家选边站队情况描述枚举类型，红方总是希望分数越大越好，而蓝方做为红方的抬杠者，总是希望分数越小越好，
    // 简单来说，当分数为 +Infinity 时，可以认为红方胜利，当分数为 -Infinity 时，可以认为蓝方胜利。
    enum class PlayerColor { RED, BLUE, TBD };

    // 玩家信息描述符
    struct Player {
        Player( ) : color(PlayerColor::TBD) { }

        // 所属战队
        PlayerColor color;

        // 直接对手
        std::shared_ptr<Player> opponent;
    };

    /**
     * MiniMax 估分函数，它基于最坏假设（对手总会做最不利于自己的选择）评估当前的格局 boardConfiguration 对于 currentPlayer 的效用分数。
     *
     * boardConfiguration - 棋局配置
     * currentPlayer - 玩家信息描述符，含有它的颜色和一个指向它的对手的玩家信息描述符的指针，
     *
     */
    template <typename T>
    double minimax(
        const T &boardConfiguration,
        const Player &currentPlayer
    ) {
        // 初始最佳分数 -Infinity（负无穷大）
        double bestScore = 0 - std::numeric_limits<double>::infinity();
        // 初始最差分数 Infinity（正无穷大）
        double worstScore = std::numeric_limits<double>::infinity();

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

    void test() {
        using PlayerColor::TBD;
        using PlayerColor::BLUE;
        using PlayerColor::RED;

        auto redPlayer = std::make_shared<Player>();
        auto bluePlayer = std::make_shared<Player>();
        redPlayer->color = RED;
        redPlayer->opponent = bluePlayer;
        bluePlayer->color = BLUE;
        bluePlayer->opponent = redPlayer;

        // 一个 3*3 棋盘，初始状态下每个位置都是 TBD（未归属）
        std::array<PlayerColor, 3*3> board { TBD, TBD, TBD, TBD, TBD, TBD, TBD, TBD, TBD };
        std::cout << "Score: " << minimax(board, *redPlayer) << std::endl;


        // 以下步骤是必须的，否则会形成循环引用，shared_ptr 基于引用计数的内存回收机制在这种情况下会失效。
        // 提示：至少 redPlayer 或者 bluePlayer 其中一个的 opponent 指针要置 null.
        redPlayer->opponent = nullptr;
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MINIMAX_HPP
