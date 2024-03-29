cmake_minimum_required(VERSION 3.22)
project(DataStructureImplementations)

set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)
find_package(nlohmann_json 3.10.5 REQUIRED)

file(
        STRINGS
        TestCases/medianOfTwoSortedArrayTestCases.txt
        MEDIAN_OF_TWO_SORTED_ARRAY_TESTCASES_CONTENT
)

configure_file(
        ${PROJECT_SOURCE_DIR}/TestCases/MedianOfTwoSortedArrayTestCase.h.in
        ${PROJECT_BINARY_DIR}/TestCases/MedianOfTwoSortedArrayTestCase.h
        @ONLY
)

add_executable(entry main.cpp DataStructures/Heap.hpp DataStructures/BinarySearchTree.hpp DataStructures/RedBlackTree.hpp Algorithms/ReverseLinkedList.hpp Algorithms/IntersectionOfTwoLinkedList.hpp Algorithms/LongestPalindromeSubString.hpp Algorithms/AddStringFormBinary.hpp Algorithms/TrapRainWater.hpp Utils/PrintVector.hpp Algorithms/SubStringSearch.hpp Algorithms/JumpGame.hpp Algorithms/JumpGameII.hpp Algorithms/LinkedListHasCycle.hpp Algorithms/TwoSum.hpp Algorithms/Sudoku.hpp Algorithms/NQueens.hpp Algorithms/Permutations.hpp Algorithms/HighlightKeywords.hpp Algorithms/DeleteElementsAppearsMoreThanOnce.hpp Algorithms/TowerOfHanoi.hpp Algorithms/MaximumRectangle.hpp Algorithms/SpiralMatrix.hpp Algorithms/BalancedBST.hpp Algorithms/ReversePolishNotationCalculator.hpp Algorithms/FirstAndLastPositionOfTarget.hpp Algorithms/Triangle.hpp Algorithms/LongestConsecutiveSequence.hpp Algorithms/MergeIntervals.hpp Algorithms/MinPathSum.hpp Utils/MakeSampleVector.hpp Interfaces/Matrix.hpp Algorithms/WildcardMatch.hpp Algorithms/QuickSort.hpp Interfaces/TestCase.hpp Algorithms/Dijkstra.hpp Utils/RandomInteger.h Algorithms/MinEditDistance.hpp Algorithms/DistinctSubsequences.hpp Algorithms/CoinChange.hpp Algorithms/WordBreak.hpp Algorithms/PerfectSquares.hpp Algorithms/Fibonacci.hpp Utils/PrintTable.hpp Algorithms/Subsets.hpp Algorithms/IsSubSequence.hpp Algorithms/WordSearch.hpp SystemDesign/MeetingScheduler.hpp Algorithms/MergeSortedLists.hpp Algorithms/GasStation.hpp Algorithms/ReOrderList.hpp Algorithms/InterleaveString.hpp Algorithms/SortColors.hpp Algorithms/HappyNumber.hpp Algorithms/MaximumSquare.hpp Algorithms/RecoverBinarySearchTree.hpp Algorithms/SimplifyPath.hpp Algorithms/SetMatrixZeroes.hpp Algorithms/RotateList.hpp SystemDesign/LRUCache.hpp Algorithms/LargestRectangleInHistogram.hpp SystemDesign/LFUCache.hpp Algorithms/CombinationSum.hpp DataStructures/RotatedSortedArray.hpp SystemDesign/FileSystem.hpp Algorithms/SameTree.hpp Algorithms/MedianOfTwoSortedArray.hpp Utils/Parser/MyTestCaseParser.hpp TestCases/MedianOfTwoTestCases.hpp Algorithms/MiniMax.hpp MetaProgramming/is_index_sequence.hpp MetaProgramming/tuple_to_array.hpp MetaProgramming/print.hpp MetaProgramming/generate_scan_lines.hpp MetaProgramming/array.hpp MetaProgramming/boolean.hpp MetaProgramming/char.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(entry PRIVATE spdlog::spdlog)

