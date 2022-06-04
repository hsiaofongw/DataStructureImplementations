#include <iostream>
#include <random>
#include "DataStructures/Heap.hpp"
#include "DataStructures/BinarySearchTree.hpp"
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

    using Handle = BST::BSTHandle<std::string, uint64_t>;
    auto handlePtr = std::make_shared<Handle>();
    std::cout << "数据：\n";
    std::for_each(std::begin(testData), std::end(testData), [&handlePtr](auto pair) {
        std::cout << "insert: " << "(" << pair.first << ", " << pair.second << ")\n";
        auto strPtr = std::make_shared<std::string>(pair.first);
        auto valPtr = std::make_shared<uint64_t>(pair.second);
        handlePtr->insert(strPtr, valPtr);
    });

    auto keyVector = std::vector<std::string> {};
    for (const auto& pair : testData) {
        keyVector.emplace_back(pair.first);
    }

    auto randomDevice = std::random_device {};
    auto randomEngine = std::default_random_engine { randomDevice() };
    auto distribution = std::bernoulli_distribution {};

    auto sampleVector = std::vector<bool> {};
    for (const auto& _ : keyVector) {
        auto sampleBit = distribution(randomEngine);
        sampleVector.push_back(sampleBit);
    }

    std::cout << "抽样表：\n";
    for (size_t idx = 0; idx < keyVector.size(); ++idx) {
        std::cout << keyVector[idx] << ": ";
        if (sampleVector[idx]) {
            std::cout << "*";
        } else {
            std::cout << " ";
        }
        std::cout << "\n";
    }

    std::cout << "搜索测试：\n";
    for (size_t idx = 0; idx < sampleVector.size(); ++idx) {
        auto sampleBit = static_cast<bool>(sampleVector[idx]);
        auto key = keyVector[idx];
        if (sampleBit) {
            std::cout << key << ": ";
            if (auto resultPtr = handlePtr->search(key)) {
                std::cout << "查询结果值 " << (*resultPtr) << " 实际值 " << testData[key];
            }
            std::cout << "\n";
        }
    }

    return 0;

    std::cout << "PreOrder: \n";
    handlePtr->traverseInOrderLNR(
            [](const auto& root) {
                    std::cout << "key: " << (*root->keyPtr) << ", value: " << (*root->valuePtr) << "\n";
            },
            [](const auto& _1, const auto& _2) -> bool { return false;  },
            [](const auto& nodePtr) -> bool { return true;  }
    );

    std::cout << "PostOrder: \n";
    handlePtr->traverseInOrderRNL(
            [](const auto &root) {
                std::cout << "key: " << (*root->keyPtr) << ", value: " << (*root->valuePtr) << "\n";
            },
            [](const auto &_1, const auto &_2) -> bool { return false; },
            [](const auto &nodePtr) -> bool { return true; }
    );

    std::cout << "Between E and R, inclusively: \n";

    // 定义下确界为 "E", 闭区间的左端点是 "E"
    auto lb = std::string { "E" };

    // 定义上确界为 "R", 闭区间的右端点是 "R"
    auto ub = std::string { "R" };

    auto resultPtr = handlePtr->rangeSearchMany(lb, ub);
    const auto& result = *resultPtr;

    std::for_each(std::begin(result), std::end(result), [](const auto& nodePtr) -> void {
        const auto& key = *nodePtr->keyPtr;
        const auto& value = *nodePtr->valuePtr;
        std::cout << "key: " << key << ", value: " << value << "\n";
    });

    std::for_each(std::begin(testData), std::end(testData), [&handlePtr](auto pair) {
        auto keyPtr = std::make_shared<std::string>(pair.first);
        auto floorNodePtr = handlePtr->floor(keyPtr);
        auto ceilNodePtr = handlePtr->ceil(keyPtr);

        std::cout << "floor(" << (*keyPtr) << ") = ";
        if (!floorNodePtr) {
            std::cout << "(nil)";
        }
        else {
            std::cout << (*floorNodePtr->keyPtr);
        }
        std::cout << ", ";
        std::cout << "ceil(" << (*keyPtr) << ") = ";
        if (!ceilNodePtr) {
            std::cout << "(nil)";
        }
        else {
            std::cout << (*ceilNodePtr->keyPtr);
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
