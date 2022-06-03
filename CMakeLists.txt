cmake_minimum_required(VERSION 3.22)
project(PriorityQueueImplementations)

set(CMAKE_CXX_STANDARD 23)

add_executable(entry main.cpp DataStructures/Heap.hpp DataStructures/BinarySearchTree.hpp)
