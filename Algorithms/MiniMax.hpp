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
     * MiniMax 估分函数，它基于最坏假设（对手总会做最不利于自己的选择）评估当前的格局对于给定玩家的效用分数，
     * 在估分过程中，它会做一些启发性的剪枝操作，跳过一些根本就无需进一步搜索的节点。
     *
     * @tparam T - 格局描述符
     * @tparam InputIt - T 的迭代器类型
     *
     * @param boardConfiguration - 棋局配置
     * @param currentPlayer - 玩家信息描述符，含有它的颜色和一个指向它的对手的玩家信息描述符的指针
     * @param getPossibleMoves - 返回给定的 player 在给定的 board 格局下所能作出的所有可能的走子的迭代器 (begin, end) 的元组
     * @param staticEvaluate - 直接根据牌面上的格局信息来估分的函数
     */
    template <typename T, class InputIt>
    double minimax(
        const T &boardConfiguration,
        const Player &currentPlayer,
        std::function<std::pair<InputIt, InputIt> (const T&, const Player&)> getPossibleMoves,
        std::function<double (const T&)> staticEvaluate
    ) {
        // 初始最佳分数 -Infinity（负无穷大）
        double bestScore = 0 - std::numeric_limits<double>::infinity();
        // 初始最差分数 Infinity（正无穷大）
        double worstScore = std::numeric_limits<double>::infinity();

        auto moves = getPossibleMoves(boardConfiguration, currentPlayer);
        if (moves.first == moves.second) {
            return staticEvaluate(boardConfiguration);
        }

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

    using Board = std::array<PlayerColor, 9>;

    class MoveIterator {
    private:
        PlayerColor color;
        Board board;

        /** 让棋子走到下一个位置 */
        void moveChessNext() {
            // 记录上一次的棋子的位置
            size_t lastPosition = this->idx;

            // 从下一个位置开始寻找
            this->idx++;
            // 两种情况 break 这个 while loop: 1) 找到一个 TBD 位置；2) 或者遍历完 board 的全部 9 个位置都没找到 TBD 位置；
            while (this->idx < 9) {
                if (board[this->idx] == PlayerColor::TBD) {
                    // 找到了
                    // 先撤回上一次的落子
                    board[lastPosition] = PlayerColor::TBD;

                    // 然后把棋子放到新的位置
                    board[this->idx] = this->color;
                    break;
                }
                ++idx;
            }
        }

    public:
        size_t idx;

        MoveIterator(const Board &board, size_t initialPosition, PlayerColor color) : idx(initialPosition), board(board), color(color) {
            this->board[initialPosition] = color;
        }

        bool operator==(const MoveIterator &rhs) const {
            return this->idx == rhs.idx;
        }

        bool operator!=(const MoveIterator &rhs) const {
            return this->idx != rhs.idx;
        }

        // 前置 ++i
        MoveIterator &operator++() {
            moveChessNext();
            return *this;
        }

        // 后置 i++
        MoveIterator operator++(int) {
            // 声明一个临时 Iterator 对象用于返回，注意 this->board 会被（也应该被）完完全全复制一份
            MoveIterator moveIt (this->board, this->idx, this->color);

            // 执行自增操作
            moveChessNext();

            return moveIt;
        }

        const Board &operator*() const {
            return this->board;
        }
    };

    void PrintBoard(const Board &board) {
        std::cout << "= = = = =" << std::endl;
        for (size_t i = 0; i < 3; ++i) {
            std::cout << "= ";
            for (size_t j = 0; j < 3; ++j) {
                auto chess = board[i*3+j];
                switch (chess) {
                    case PlayerColor::RED:
                        std::cout << "X ";
                        break;
                    case PlayerColor::BLUE:
                        std::cout << "O ";
                        break;
                    default:
                        std::cout << "  ";
                }
            }
            std::cout << "=" << std::endl;
        }
        std::cout << "= = = = =" << std::endl;
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
        std::array<PlayerColor, 3*3> board { BLUE, TBD, TBD, TBD, TBD, TBD, TBD, TBD, TBD };

        MoveIterator moveIt (board, 1, PlayerColor::RED);
        for (size_t i = 0; i < 8; ++i) {
            std::cout << "i: " << i << std::endl;
            PrintBoard(*moveIt);
            ++moveIt;
        }

        // 以下步骤是必须的，否则会形成循环引用，shared_ptr 基于引用计数的内存回收机制在这种情况下会失效。
        // 提示：至少 redPlayer 或者 bluePlayer 其中一个的 opponent 指针要置 null.
        redPlayer->opponent = nullptr;
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MINIMAX_HPP
