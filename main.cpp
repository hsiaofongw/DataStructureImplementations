#include <iostream>
#include "SystemDesign/LFUCache.hpp"
#include <utility>
#include "SystemDesign/FileSystem.hpp"

/**
 [[2],[2,1],[3,2],[3],[2],[4,3],[2],[3],[4]]
 */

int main() {
    using SystemDesign::FileSystem::LeetCode::FileSystem;
    FileSystem fs;
    fs.ls("/");
    fs.mkdir("/a/b/c");
    fs.addContentToFile("/a/b/c/d", "hello");
    fs.ls("/");
    auto content = fs.readContentFromFile("/a/b/c/d");

    return 0;
}
