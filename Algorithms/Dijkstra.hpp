//
// Created by 韦晓枫 on 2022/7/22.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_DIJKSTRA_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_DIJKSTRA_HPP

#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <queue>

namespace Algorithm {
    namespace DijkstraShortestPathDistanceAlgorithm {
        /**
         * Dijkstra 最短路径距离算法
         *
         * 主要思路：
         *
         * 要知道一个顶点 s 到顶点 v 的最小距离 minDist(s, v), 可以考虑
         * 所有下一跳就是 v 的节点 w1, w2, ..., w_n, 假如说我们知道：
         * minDist(s, w_k) <= 所有其它的 minDist(s, w_i), i = 1,2,3, ..., n
         * 那么 minDist(s, v) = minDist(s, w_k) + w_k 到 v 的距离。
         */

        /* 节点 ID */
        using NodeId = size_t;

        /** 距离, double 可表示正、负数，还可表示 0, 正无穷大和负无穷大 */
        using Distance = double;

        /** 连接列表 */
        using Connections = std::unordered_map<NodeId, Distance>;

        /** 距离矩阵 */
        using DistanceMatrix = std::unordered_map<NodeId, Connections>;

        /**
         * 参数说明：
         * 1. distance[i] 表示节点 i 与节点 i 的邻居的连接权值；
         * 2. 如果 distance[j] 是一个默认构造的 Connections 对象的引用，则表示节点 j 没有到下一跳的连接；
         * 3. start 表示我们要算 id 为 start 的节点到其它节点的最短距离；
         * 4. start 到其它节点的最短距离用 minDist[start] 存储，
         *    譬如说：minDist[start][4] = 5 则表示 start 到节点 4 的最短距离是 5;
         * 5. 使用时，可以默认构造一个空的 DistanceMatrix 对象，然后让 minDist 引用这个对象即可；
         */
        void calculateMinDistances(
            DistanceMatrix &distance,
            NodeId start,
            DistanceMatrix &minDist
        ) {
            constexpr auto PositiveInfinity = std::numeric_limits<bool>::infinity();
            for (const auto &pair : distance) {
                NodeId nodeId = pair.first;
                minDist[start][nodeId] = PositiveInfinity;
            }
            minDist[start][start] = 0;

            std::queue<NodeId> candidates;
            candidates.push(start);
            std::unordered_set<NodeId> traversed;
            while (!candidates.empty()) {
                NodeId currentNodeId = candidates.front();
                candidates.pop();
                traversed.insert(currentNodeId);
                for (const auto &adjacency : distance[currentNodeId]) {
                    NodeId adjacencyNodeId = adjacency.first;
                    Distance fromCurrentToAdjacency = adjacency.second;
                    if (!traversed.contains(adjacencyNodeId)) {
                        candidates.push(adjacencyNodeId);
                    }

                    Distance fromStartToAdj = minDist[start][adjacencyNodeId];
                    Distance fromStartToAdjViaCurrentNode = minDist[start][currentNodeId] + fromCurrentToAdjacency;

                    if (fromStartToAdj > fromStartToAdjViaCurrentNode) {
                        minDist[start][adjacencyNodeId] = fromStartToAdjViaCurrentNode;
                    }
                }
            }
        }
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_DIJKSTRA_HPP
