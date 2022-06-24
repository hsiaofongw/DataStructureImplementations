cmake_minimum_required(VERSION 3.22)
project(DataStructureImplementations)

set(CMAKE_CXX_STANDARD 23)

add_executable(entry main.cpp DataStructures/Heap.hpp DataStructures/BinarySearchTree.hpp DataStructures/RedBlackTree.hpp Algorithms/ReverseLinkedList.hpp Algorithms/IntersectionOfTwoLinkedList.hpp)
