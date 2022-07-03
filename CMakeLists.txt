cmake_minimum_required(VERSION 3.22)
project(DataStructureImplementations)

set(CMAKE_CXX_STANDARD 23)

find_package(spdlog REQUIRED)

add_executable(entry main.cpp DataStructures/Heap.hpp DataStructures/BinarySearchTree.hpp DataStructures/RedBlackTree.hpp Algorithms/ReverseLinkedList.hpp Algorithms/IntersectionOfTwoLinkedList.hpp Algorithms/LongestPalindromeSubString.hpp Algorithms/AddStringFormBinary.hpp Algorithms/TrapRainWater.hpp Utils/PrintVector.hpp Algorithms/SubStringSearch.hpp)

target_link_libraries(entry PRIVATE spdlog::spdlog)

