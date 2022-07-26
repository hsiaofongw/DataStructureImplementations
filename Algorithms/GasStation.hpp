//
// Created by 韦晓枫 on 2022/7/26.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_GASSTATION_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_GASSTATION_HPP

#include <vector>
#include <deque>

namespace Algorithm::GasStation {

    struct TestCase {
        std::vector<int> gas;
        std::vector<int> cost;
        int expectedOutput;
    };

    std::vector<TestCase> getTestCases() {
        return {
                TestCase {
                        .gas = { 1,2,3,4,5 },
                        .cost = { 3,4,5,1,2 },
                        .expectedOutput = 3
                },
            TestCase {
                .gas = { 2 },
                .cost = { 2 },
                .expectedOutput = 0
                },

                TestCase {
                    .gas = { 2,3,4 },
                    .cost = { 3,4,3 },
                    .expectedOutput = -1
                }
        };
    }

    /**
     * 小车车能够从 startIdx 开到 (startIdx+len+1)%N,
     * 取决于小车能否从 startIdx 开到 (startIdx+len)%N 并且小车的油量加上当前加油站的油量能够开到下一站。
     */
    class Solution {
    public:
        int canCompleteCircuit(std::vector<int>& gas, std::vector<int>& cost) {
            size_t N = gas.size();
            assert((N >= 1));

            std::vector<int> travelLength (N, 0);
            std::vector<int> tank (N, 0);
            std::deque<size_t> increasedIndices;
            int totalGas = 0;
            int totalCost = 0;
            for (size_t i = 0; i < N; ++i) {
                totalGas += gas[i];
                totalCost += cost[i];
                if (gas[i] >= cost[i]) {
                    travelLength[i] = 1;
                    tank[i] = gas[i] - cost[i];
                    increasedIndices.push_back(i);
                }
            }

            if (totalCost > totalGas) {
                return -1;
            }

            while (!increasedIndices.empty()) {
                size_t startAt = increasedIndices.front();

                if (increasedIndices.size() == 1) {
                    return static_cast<int>(startAt);
                }

                increasedIndices.pop_front();

                size_t terminal = (startAt + travelLength[startAt])%N;
                if (travelLength[terminal] > 0) {
                    travelLength[startAt] += travelLength[terminal];
                    increasedIndices.push_back(startAt);
                    tank[startAt] += tank[terminal];
                } else if (tank[startAt] + gas[terminal] - cost[terminal] >= 0) {
                    travelLength[startAt] += 1;
                    tank[startAt] += gas[terminal] - cost[terminal];
                    increasedIndices.push_back(startAt);
                }

                if (travelLength[startAt] == N)
                    return static_cast<int>(startAt);
            }

            // 如果找不到这样的能够支持小车车开一整圈的起始位置，返回 -1
            return -1;
        }
    };
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_GASSTATION_HPP
