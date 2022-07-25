//
// Created by 韦晓枫 on 2022/7/26.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_GASSTATION_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_GASSTATION_HPP

#include <vector>

namespace Algorithm::GasStation {

    /**
     * 开车🚗问题 DP 解法：
     * 小车车能够从 startIdx 开到 (startIdx+len+1)%N,
     * 取决于小车能否从 startIdx 开到 (startIdx+len)%N 并且小车的油量加上当前加油站的油量能够开到下一站。
     */
    class Solution {
    public:
        int canCompleteCircuit(std::vector<int>& gas, std::vector<int>& cost) {
            size_t N = gas.size();
            assert((N >= 1));
            std::vector<std::vector<uint8_t>> canTravel (gas.size());
            for (auto &row : canTravel)
                row.resize(N+1, 0);

            // canTravel[i][j]: Can travel from index i to index j ?
            // 遍历每个起始位置
            for (size_t startAt = 0; startAt < N; ++startAt) {
                int tank = 0;  // 初始化小车车🚗的油箱
                canTravel[startAt][startAt] = 1;  // 把小车车🚗放在起始位置上
                for (size_t len = 1; len <= N; ++len) {
                    // 开小车车🚗，一步一步地开，看能开多远
                    size_t currentIdx = (startAt + len - 1) % N; // 当前小车车🚗在这里
                    size_t travelToIdx = (startAt + len) % N;  // 小车车🚗接下来要去这里
                    tank = tank + gas[currentIdx];  // 小车车🚗出发前先加满油
                    if (canTravel[startAt][currentIdx] == 1 && tank > cost[currentIdx]) {
                        // 如果小车车🚗能够从开始位置到达这里, 并且小车车🚗有足够的油跑到下一站
                        // 那么标记小车车🚗能够从一开始跑到下一站
                        canTravel[startAt][travelToIdx] = 1;

                        // 并且把小车车🚗开过去
                        tank = tank - cost[currentIdx];
                    } else {
                        // 否则就标记小车车🚗不能够从一开始开到下一站
                        canTravel[startAt][travelToIdx] = 0;
                    }
                }
            }

            // 遍历每个起始位置
            for (size_t startAt = 0; startAt < N; ++startAt) {
                // 如果小车车🚗能够开一圈
                if (canTravel[startAt][(startAt+N)%N]) {
                    // 那么就返回这个能支持小车车🚗开一圈的起始位置
                    return static_cast<int>(startAt);
                }
            }

            // 如果找不到这样的能够支持小车车开一整圈的起始位置，返回 -1
            return -1;
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_GASSTATION_HPP
