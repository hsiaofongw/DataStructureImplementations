#include <iostream>
#include <random>
#include "heap.hpp"
#include "BinarySearchTree.hpp"
#include <unordered_map>
#include <map>

bool verifyTwoNumberListIdentical(const std::vector<uint64_t>& lst1, const std::vector<uint64_t>& lst2) {
    auto lst1Stat = std::unordered_map<uint64_t, size_t> {};
    auto lst2Stat = std::unordered_map<uint64_t, size_t> {};

    for (auto &num : lst1) {
        auto count = lst1Stat[num];
        lst1Stat[num] = count + 1;
    }

    for (auto &num : lst2) {
        auto count = lst2Stat[num];
        lst2Stat[num] = count + 1;
    }

    return lst1Stat == lst2Stat;
}

int main() {

    auto testData = std::unordered_map<std::string, uint64_t> {
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

    auto handlePtr = std::make_shared<BST::BSTHandle<std::string, uint64_t>>();

    std::for_each(std::begin(testData), std::end(testData), [&handlePtr](auto pair) {
        std::cout << "insert: " << "key: " << pair.first << ", value: " << pair.second << "\n";
        auto strPtr = std::make_shared<std::string>(pair.first);
        auto valPtr = std::make_shared<uint64_t>(pair.second);
        handlePtr->insert(strPtr, valPtr);
    });

    std::for_each(std::begin(testData), std::end(testData), [&handlePtr](auto pair) {
        auto keyPtr = std::make_shared<std::string>(pair.first);
        auto floorNodePtr = handlePtr->floor(keyPtr);

        std::cout << "floor(" << (*keyPtr) << ") = ";
        if (!floorNodePtr) {
            std::cout << "(nil)";
        }
        else {
            std::cout << (*floorNodePtr->keyPtr);
        }
        std::cout << "\n";
    });

//    while (!handlePtr->empty()) {
//        auto minNodePtr = handlePtr->max();
//        std::cout << "read: " << "key: " << (*minNodePtr->keyPtr) << ", value: " << (*minNodePtr->valuePtr) << "\n";
//        handlePtr->deleteMax();
//    }


//    std::random_device randomDevice;  //Will be used to obtain a seed for the random number engine
//    std::mt19937 randomEngine(randomDevice()); //Standard mersenne_twister_engine seeded with randomDevice()
//    std::uniform_int_distribution<> distribution(1, 60);
//
//    constexpr size_t sampleSize = 100;
//    std::vector<uint64_t> nums {};
//
//    while (nums.size() < sampleSize) {
//        nums.push_back(static_cast<uint64_t>(distribution(randomEngine)));
//    }
//
//    std::cout << "Origin: ";
//    printVector(nums);
//
//    auto heapPtr = std::make_unique<Heap<uint64_t>>(
//            std::vector<uint64_t> (nums),
//            [](const uint64_t& a, const uint64_t& b) { return a <= b; }
//    );
//
//    std::cout << "Sorted: ";
//    auto sortedNums = std::vector<uint64_t> {};
//    while (!heapPtr->empty()) {
//        sortedNums.push_back(heapPtr->top());
//        heapPtr->pop();
//    }
//
//    printVector(sortedNums);
//
//    std::cout << "Verified: " << verifyTwoNumberListIdentical(nums, sortedNums);

    return 0;
}
