cmake_minimum_required(VERSION 3.22)
project(DataStructureImplementations)

set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)

add_executable(entry main.cpp DataStructures/Heap.hpp DataStructures/BinarySearchTree.hpp DataStructures/RedBlackTree.hpp Algorithms/ReverseLinkedList.hpp Algorithms/IntersectionOfTwoLinkedList.hpp Algorithms/LongestPalindromeSubString.hpp Algorithms/AddStringFormBinary.hpp Algorithms/TrapRainWater.hpp Utils/PrintVector.hpp Algorithms/SubStringSearch.hpp Algorithms/JumpGame.hpp Algorithms/JumpGameII.hpp Algorithms/LinkedListHasCycle.hpp Algorithms/TwoSum.hpp Algorithms/Sudoku.hpp Algorithms/NQueens.hpp Algorithms/Permutations.hpp Algorithms/HighlightKeywords.hpp Algorithms/DeleteElementsAppearsMoreThanOnce.hpp Algorithms/TowerOfHanoi.hpp Algorithms/MaximumRectangle.hpp Algorithms/SpiralMatrix.hpp Algorithms/BalancedBST.hpp Algorithms/ReversePolishNotationCalculator.hpp Algorithms/FirstAndLastPositionOfTarget.hpp Algorithms/Triangle.hpp Algorithms/LongestConsecutiveSequence.hpp Algorithms/MergeIntervals.hpp Algorithms/MinPathSum.hpp Utils/MakeSampleVector.hpp Interfaces/Matrix.hpp Algorithms/WildcardMatch.hpp Algorithms/QuickSort.hpp Interfaces/TestCase.hpp Algorithms/Dijkstra.hpp Utils/RandomInteger.h Algorithms/MinEditDistance.hpp Algorithms/DistinctSubsequences.hpp Algorithms/CoinChange.hpp)

target_link_libraries(entry PRIVATE spdlog::spdlog)

