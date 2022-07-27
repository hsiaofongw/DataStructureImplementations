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
#include "Algorithms/MinEditDistance.hpp"
#include "Algorithms/DistinctSubsequences.hpp"
#include "Algorithms/CoinChange.hpp"
#include "Algorithms/WordBreak.hpp"
#include "Algorithms/Fibonacci.hpp"
#include "Utils/PrintTable.hpp"
#include "Algorithms/PerfectSquares.hpp"
#include <array>
#include "Algorithms/Subsets.hpp"
#include <algorithm>
#include "Algorithms/IsSubSequence.hpp"
#include "Algorithms/WordSearch.hpp"
#include "SystemDesign/MeetingScheduler.hpp"
#include "Algorithms/GasStation.hpp"
#include "Algorithms/MaximumSquare.hpp"
#include "Algorithms/SimplifyPath.hpp"
#include "SystemDesign/LRUCache.hpp"

int main() {

    {
        using SystemDesign::LRUCache::LRUCache;

        constexpr int cacheCapacity = 2;
        LRUCache lruCache (cacheCapacity);

        auto getKey = [&lruCache](int key) -> void {
            std::cout << "get(key = " << key << "): " << lruCache.get(1) << "\n";
        };

        lruCache.put(1, 1);
        lruCache.put(2, 2);
        getKey(1);
        lruCache.put(3, 3);
        getKey(2);
        lruCache.put(4, 4);
        getKey(1);
        getKey(3);
        getKey(4);
    }

    return 0;

    {
        using Algorithm::SimplifyPath::getTestCases;
        using Algorithm::SimplifyPath::Solution;

        Solution solution;
        for (auto &testCase : getTestCases()) {
            std::string actual = solution.simplifyPath(testCase.path);
            std::cout << "Input: " << testCase.path << "\n";
            std::cout << "Output: " << actual << "\n";
            std::cout << "Expected: " << testCase.normalizedPath << "\n";
            assert((actual == testCase.normalizedPath));
        }
    }

    return 0;

    {
        using Algorithm::MaximumSquare::Solution;
        using Algorithm::MaximumSquare::getTestCases;

        Solution solution;
        for (auto &testCase : getTestCases()) {
            auto result = solution.maximalSquare(testCase.matrix);
            std::cout << "Actual: " << result << ", Expected: " << testCase.expectedOutput << "\n";
            assert((result == testCase.expectedOutput));
        }
    }

    return 0;

    {
        using Algorithm::GasStation::Solution;
        using Algorithm::GasStation::getTestCases;

        Solution solution;
        for (auto &testCase : getTestCases()) {
            auto result = solution.canCompleteCircuit(testCase.gas, testCase.cost);
            std::cout << "Actual: " << result << ", " << "Expected: " << testCase.expectedOutput << "\n";
            assert((result == testCase.expectedOutput));
        }
    }

    return 0;

    {
        using SystemDesign::MeetingScheduler::Scheduler;
        Scheduler scheduler;
        auto meeting1 = scheduler.scheduleMeeting(2);
        scheduler.printSlots();
        auto meeting2 = scheduler.scheduleMeeting(4);
        scheduler.printSlots();

        if (meeting1) {
            scheduler.cancel(*meeting1);
            scheduler.printSlots();
        }
    }

    return 0;

    {
        using Algorithm::WordSearch::Solution;
        Solution solution;
        for (const auto &testCase : solution.getTestCases()) {
            bool output = solution.exist(testCase.matrix, testCase.word);
            std::cout << "Actual: " << std::boolalpha << output << ", Expected: " << std::boolalpha << testCase.expectedOutput << "\n";
            assert((output == testCase.expectedOutput));
        }
    }

    return 0;

    {
        using Algorithm::IsSubSequence::getTestCases;
        using Algorithm::IsSubSequence::Solution;

        Solution solution;
        for (auto &testCase : getTestCases()) {
            auto output = solution.isSubsequence(testCase.subSequence, testCase.text);
            std::cout << "Actual: " << std::boolalpha << output << ", Expected: " << std::boolalpha << testCase.expectedOutput << "\n";
            assert((output == testCase.expectedOutput));
        }
    }

    return 0;

    {
        using Algorithm::Subsets::getTestCases;
        using Algorithm::Subsets::Solution;

        Solution solution;
        for (auto &testCase : getTestCases()) {
            auto subsets = solution.subsets(testCase.nums);
            for (auto &subset : subsets) {
                std::sort(subset.begin(), subset.end());
            }

            std::sort(subsets.begin(), subsets.end(), [](const std::vector<int> &subsetA, const std::vector<int> &subsetB) -> bool {
                return subsetA < subsetB;
            });

            auto expectedSubsets = testCase.subsets;
            for (auto &subset : expectedSubsets) {
                std::sort(subset.begin(), subset.end());
            }

            std::sort(expectedSubsets.begin(), expectedSubsets.end(), [](const std::vector<int> &a, const std::vector<int> &b) -> bool {
                return a < b;
            });

            assert((subsets == expectedSubsets));
        }
    }

    return 0;

    {
        std::vector<int64_t> index { 11, 1234, 151 };
        std::vector<int64_t> header { 161, 1, 2, 4 };
        std::vector<std::vector<int64_t>> cells = {
            { 1, 2, 3, 4 },
            { 5, 6, 7, 89999 },
            { 110, 11, 13, 14 }
        };

        Utils::PrintTable(index, header, cells);
    }

    return 0;

    {
        using Algorithm::Fibonacci::Solution;
        Solution solution;
        for (uint32_t n = 1; n <= 14; ++n) {
            auto val = solution.getNth(n);
            std::cout << "F(" << n << ") = " << val << "\n";
        }

        for (uint32_t n = 4; n <= 10; ++n) {
            auto val = solution.getNth(n);
            std::cout << "F(" << n << ") = " << val << "\n";
        }
    }

    return 0;

    {
        using Algorithm::WordBreak::getTestCases;
        using Algorithm::WordBreak::Solution;

        Solution solution;
        for (auto &testCase : getTestCases()) {
            bool result = solution.wordBreak(testCase.s, testCase.wordDict);
            bool expected = testCase.expected;
            std::cout << "Result: " << std::boolalpha << result
                << ", Expected: " << std::boolalpha << expected << "\n";
            assert((result == expected));
        }
    }

    return 0;

    {
        using Algorithm::CoinChange::getTestCases;
        using Algorithm::CoinChange::Solution;

        for (auto &testCase : getTestCases()) {
            Solution solution;
            auto result = solution.coinChange(testCase.coins, testCase.amount);
            std::cout << "Actual: " << result << ", " << "Expected: " << testCase.expectedOutput << "\n";
            assert((result == testCase.expectedOutput));
        }
    }

    return 0;

    {
        using Algorithm::SubStringSearch::KMPStringMatcher;
        using Algorithm::SubStringSearch::getTestCases;

        for (const auto &testCase : getTestCases()) {
            KMPStringMatcher matcher (testCase.pattern);
            auto result = matcher.search(testCase.text);
            std::cout << "Found idx: " << result << ", "
                << "Expected at: " << testCase.expectedAt << "\n";
            assert((result == testCase.expectedAt));
        }
    }

    return 0;

    {
        using Algorithm::DistinctSubsequences::getTestCases;
        using Algorithm::DistinctSubsequences::numDistinct;

        for (const auto &testCase : getTestCases()) {
            auto actual = numDistinct(testCase.originString, testCase.subSequence);
            assert((actual == testCase.desiredOutput));
        }

    }

    return 0;


    {
        using Algorithm::MinEditDistance::getMinEditDistance;
        using Algorithm::MinEditDistance::getTestCases;

        auto testCases = getTestCases();
        for (const auto &testCase : testCases) {
            size_t actual = getMinEditDistance(testCase.lhs, testCase.rhs);
            assert((actual == testCase.expectedOutput));
        }
    }

    return 0;

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
