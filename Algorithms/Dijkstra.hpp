//
// Created by 韦晓枫 on 2022/7/22.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_DIJKSTRA_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_DIJKSTRA_HPP

#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <queue>
#include <memory>
#include <fstream>

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

        /** 有向图构造器 */
        class DirectedGraphBuilder {
        public:
            /** 添加节点 */
            void addVertex(NodeId nodeId) {
                this->adjacency[nodeId];
            }

            /** 连接节点 */
            void connect(NodeId from, NodeId to) {
                this->addVertex(from);
                this->addVertex(to);
                this->adjacency[from][to];
            }

            /** 连接节点并赋权，也可以用来更新连接权值。 */
            void connect(NodeId from, NodeId to, double weight) {
                this->addVertex(from);
                this->addVertex(to);
                this->adjacency[from][to] = weight;
            }

            /** 清空内部的图的数据的暂存区 */
            void clear() {
                this->adjacency.clear();
            }

            /** 导出图 */
            std::unique_ptr<DistanceMatrix> dump() {
                return std::make_unique<DistanceMatrix>(this->adjacency);
            }

        private:
            DistanceMatrix adjacency;
        };

        /**
         * 此 struct 用于描述一个图：
         * - 它有多少个节点：nVertices
         * - 多少条边：nEdges
         * - 节点和节点之间的连接性如何：adjacency
         */
        struct GraphDescriptor {
            uint32_t nVertices;
            uint32_t nEdges;
            std::unique_ptr<DistanceMatrix> adjacency;
        };

        /**
         * 此 struct 用于描述一个 shortestPathDistance 的测试用例：
         * - 作为输入的图描述对象：graphDescriptor
         * - 正确的 0 到其它节点之间的最短距离：correctMinDist
         */
        struct TestCase {
            GraphDescriptor graphDescriptor;
            std::unique_ptr<DistanceMatrix> correctMinDist;
        };

        /** 以特定方式解析一个图数据文件 */
        decltype(auto) parseGraphData(std::ifstream &graphDataFileStream, size_t linesCount) {
            DirectedGraphBuilder graphBuilder;
            for (size_t lineIdx = 0; lineIdx < linesCount; ++lineIdx) {
                NodeId from = 0;
                NodeId to = 0;
                Distance weight = 0;

                assert((graphDataFileStream.good()));
                graphDataFileStream >> from;

                assert((graphDataFileStream.good()));
                graphDataFileStream >> to;

                assert((graphDataFileStream.good()));
                graphDataFileStream >> weight;

                graphBuilder.connect(from, to, weight);
            }

            return graphBuilder.dump();
        }

        /**
         * 加载工作目录下的 SampleData/tinyEWD.txt,
         * 从该文件的文本数据解析得到 GraphDescriptor 形式的图数据。
         */
        TestCase loadTestCase() {
            std::string graphDataFileRelPath = "SampleData/tinyEWD/tinyEWD.txt";
            std::ifstream graphDataStream (graphDataFileRelPath, std::ios::in);
            assert((graphDataStream.is_open()));

            uint32_t verticesCount = 0;
            uint32_t edgesCount = 0;
            graphDataStream >> verticesCount;
            graphDataStream >> edgesCount;

            std::string correctMinDistFileRelPath = "SampleData/tinyEWD/minDist.txt";
            std::ifstream minDistFileStream (correctMinDistFileRelPath, std::ios::in);
            assert((minDistFileStream.is_open()));

            return TestCase {
                .graphDescriptor = GraphDescriptor {
                    .nVertices = verticesCount,
                    .nEdges = edgesCount,
                    .adjacency = parseGraphData(graphDataStream, edgesCount)
                },
                .correctMinDist = parseGraphData(minDistFileStream, verticesCount)
            };
        }

        /**
         * 功能说明：
         * 此函数接受一个邻接矩阵形式编码的图 g, 设 g 有 V 个节点，且 V >= 1.
         * 然后此函数会计算 g 从起始节点 start 到其它节点的最短距离：
         * minDist[start][i], i = 0, 1, ..., V-1,
         * 并且将计算结果通过引用存储在 minDist 参数中。
         *
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
            constexpr double PositiveInfinity = std::numeric_limits<double>::infinity();
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
