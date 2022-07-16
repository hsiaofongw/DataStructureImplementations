cmake_minimum_required(VERSION 3.22)
project(DataStructureImplementations)

set(CMAKE_CXX_STANDARD 23)

find_package(spdlog REQUIRED)

add_executable(entry main.cpp DataStructures/Heap.hpp DataStructures/BinarySearchTree.hpp DataStructures/RedBlackTree.hpp Algorithms/ReverseLinkedList.hpp Algorithms/IntersectionOfTwoLinkedList.hpp Algorithms/LongestPalindromeSubString.hpp Algorithms/AddStringFormBinary.hpp Algorithms/TrapRainWater.hpp Utils/PrintVector.hpp Algorithms/SubStringSearch.hpp Algorithms/JumpGame.hpp Algorithms/JumpGameII.hpp Algorithms/LinkedListHasCycle.hpp Algorithms/TwoSum.h Algorithms/Sudoku.hpp Algorithms/NQueens.hpp Algorithms/Permutations.hpp Algorithms/HighlightKeywords.hpp Algorithms/DeleteElementsAppearsMoreThanOnce.hpp Algorithms/TowerOfHanoi.hpp Algorithms/MaximumRectangle.hpp Algorithms/SpiralMatrix.hpp)

target_link_libraries(entry PRIVATE spdlog::spdlog)

