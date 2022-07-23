#include <iostream>
#include <map>
#include <set>
#include "DataStructures/RedBlackTree.hpp"
#include "Algorithms/TrapRainWater.hpp"
#include "Utils/PrintVector.hpp"
#include "Algorithms/SubStringSearch.hpp"
#include "Algorithms/NQueens.hpp"
#include "Algorithms/MaximumRectangle.hpp"
#include "Algorithms/SpiralMatrix.hpp"
#include "Algorithms/LongestConsecutiveSequence.hpp"
#include "Algorithms/MergeIntervals.hpp"
#include "Algorithms/MinPathSum.hpp"
#include "Algorithms/MaximumRectangle.hpp"
#include "Algorithms/QuickSort.hpp"
#include <fstream>
#include <string>
#include "Algorithms/Dijkstra.hpp"
#include <algorithm>
#include "Utils/RandomInteger.h"

int main() {
    {
        using Algorithm::Sorting::getTestCases;
        using Algorithm::Sorting::quickSort;
        using Algorithm::Sorting::Comparator;

        auto testCases = getTestCases();
        Comparator<int32_t> comparator = [](const int32_t &a, const int32_t &b) -> bool {
            return a <= b;
        };
        for (auto &testCase : testCases) {
            quickSort(testCase.question, 0, testCase.question.size(), comparator);
            assert((testCase.question == testCase.expectedOutput));
        }
    }

    return 0;

    {
        using Utils::RandomIntegerGenerator;

        std::vector<std::pair<int32_t, int32_t>> randParams = {
                {0, 0},
                {0, 1},
                {0, 3},
                {2, 3},
                {3, 3},
                {0, 10},
                {-4, -3},
                {-5, -5}
        };

        for (const auto &pair : randParams) {
            RandomIntegerGenerator<int32_t> randIntGen (pair.first, pair.second);
            std::cout << "a = " << pair.first << ", " << "b = " << pair.second << "\n";
            for (size_t i = 0; i < 10; ++i) {
                std::cout << randIntGen.get() << "\n";
            }
        }
    }

    return 0;

    {
        using Algorithm::DijkstraShortestPathDistanceAlgorithm::loadTestCase;
        using Algorithm::DijkstraShortestPathDistanceAlgorithm::NodeId;
        using Algorithm::DijkstraShortestPathDistanceAlgorithm::DistanceMatrix;
        using Algorithm::DijkstraShortestPathDistanceAlgorithm::calculateMinDistances;
        using Algorithm::DijkstraShortestPathDistanceAlgorithm::Distance;

        auto testCase = loadTestCase();

        DistanceMatrix minDist;
        NodeId startNodeId = 0;
        calculateMinDistances(*(testCase.graphDescriptor.adjacency), startNodeId, minDist);
        DistanceMatrix &correctMinDist = *(testCase.correctMinDist);
        for (const auto &pair : minDist[startNodeId]) {
            Distance actual = pair.second;
            Distance expected = correctMinDist[startNodeId][pair.first];
            std::cout << "minDist"
                << "(" << startNodeId << ", " << pair.first << "): "
                << "actual: " << actual << ", "
                << "expected: " << expected << "\n";

            assert((std::abs(actual - expected) < 0.001));
        }

        std::cout << "";
    }

    return 0;

    {
        using Algorithm::MaximumRectangle::getTestCases;
        using Algorithm::MaximumRectangle::Solution;

        Solution solution;
        auto testCases = getTestCases();
        size_t caseId = 0;
        for (auto &testCase : testCases) {
            int result = solution.maximalRectangle(testCase.first);
            int expected = testCase.second;
            std::cout << "CaseId: " << caseId++ << "\n";
            std::cout << "Output: " << result << "\n";
            std::cout << "Expected: " << expected << "\n";
            assert((result == expected));
        }
    }

    return 0;

    {
        using Algorithm::MinPathSum::getTestCases;
        using Algorithm::MinPathSum::Solution;

        Solution solution;
        auto testCases = getTestCases();
        for (std::pair<Interface::IntMatrix, int> &testCase : testCases) {
            auto result = solution.minPathSum(testCase.first);
            auto expected = testCase.second;
            assert((result == expected));
        }
    }

    return 0;

    {
        using Algorithm::MergeIntervals::getTestCases;
        using Algorithm::MergeIntervals::Solution;

        Solution solution;
        auto testCases = getTestCases();
        for (auto &testCase : testCases) {
            const auto &expected = testCase.second;
            auto result = solution.merge(testCase.first);
            assert((result == expected));
        }
    }

    return 0;


    {
        using Algorithm::LongestConsecutiveSequence::getTestCases;
        using Algorithm::LongestConsecutiveSequence::Solution;

        auto testCases = getTestCases();
        for (const auto &testCase : testCases) {
            int expected = testCase.second;
            int result = Solution::longestConsecutive(testCase.first);
            assert((expected == result));
        }
    }

    return 0;



    for (size_t i = 0; i < 8; ++i) {
        const size_t problemSizeN = i+1;
        auto solutions = Algorithm::NQueens::getNQueensSolutions(problemSizeN);
        if (solutions.empty()) {
            std::cout << "No solutions for N = " << problemSizeN << ".\n";
        } else {
            std::cout << "Solutions for N = " << problemSizeN << ":\n";
            for (const auto &solution : solutions) {
                auto solutionView = Algorithm::NQueens::solutionToString(solution, problemSizeN);
                std::cout << solutionView << "\n";
            }
        }
    }

    return 0;

    using namespace DataStructure::RedBlackTree;

    auto testKeyValuePairs = std::vector<std::pair<std::string, uint64_t>> {
            { "S", 0 },
            { "E", 1 },
            { "A", 2 },
            { "R", 3 },
            { "C", 4 },
            { "H", 5 },
            { "E", 6 },
            { "X", 7 },
            { "A", 8 },
            { "M", 9 },
            { "P", 10 },
            { "L", 11 },
            { "E", 12 }
    };

    auto uniqueKeys = std::set<std::string> {};
    for (const auto &pair : testKeyValuePairs) {
        uniqueKeys.insert(pair.first);
    }

    std::cout << "Unique Keys: " << uniqueKeys.size() << "\n";

    using SimpleKeyType = std::string;
    using SimpleValueType = uint64_t;
    using SimpleRedBlackNode = RedBlackNode<SimpleKeyType, SimpleValueType>;
    using SimpleRedBlackNodePtr = std::shared_ptr<SimpleRedBlackNode>;
    using SimpleRedBlackHandle = RedBlackTreeHandle<SimpleKeyType, SimpleValueType>;

    auto root = SimpleRedBlackNodePtr {};
    for (const auto& pair : testKeyValuePairs) {
        auto keyPtr = std::make_shared<SimpleKeyType>(pair.first);
        auto valuePtr = std::make_shared<SimpleValueType>(pair.second);
        std::cout << "Insert: (" << pair.first << ", " << pair.second << ")\n";
        root = SimpleRedBlackHandle::insert(root, keyPtr, valuePtr);
        auto checkIntact = SimpleRedBlackHandle::debugCheckDefinition(root, true);
        assert((checkIntact));
        SimpleRedBlackHandle::debugPrintTreeExpr(root);
        std::cout << "\n";
        std::cout << "Size: " << root->size << "\n";
    }

    SimpleRedBlackHandle::debugCheckDefinition(root);

    std::cout << "Start deleteMin test:\n";
    std::cout << "Before deleteMin:\n";
    SimpleRedBlackHandle::debugPrintTreeExpr(root);
    std::cout << "\n";
    std::cout << "Size: " << (root ? root->size : 0) << "\n";

    for (size_t i = 0; i < uniqueKeys.size(); ++i) {
        std::cout << "deleteMin called " << i << " time(s):\n";
        root = SimpleRedBlackHandle::deleteMin(root);
        std::cout << "After deleteMin:\n";
        std::cout << "Size: " << (root ? root->size : 0) << "\n";
        SimpleRedBlackHandle::debugPrintTreeExpr(root);
        std::cout << "\n";
        auto checkIntact = SimpleRedBlackHandle::debugCheckDefinition(root, true);
        assert((checkIntact));
    }

    return 0;
}
