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

int main() {
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

    std::vector<std::vector<char>> testCase0 = {
            { '1', '0' },
            { '1', '0' }
    };

    auto answer = Algorithm::MaximumRectangle::Solution::maximalRectangle(testCase0);
    std::cout << answer << "\n";

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
