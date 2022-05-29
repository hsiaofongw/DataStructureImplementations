#include <iostream>
#include <random>
#include "heap.hpp"
#include <unordered_map>

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
    std::random_device randomDevice;  //Will be used to obtain a seed for the random number engine
    std::mt19937 randomEngine(randomDevice()); //Standard mersenne_twister_engine seeded with randomDevice()
    std::uniform_int_distribution<> distribution(1, 60);

    constexpr size_t sampleSize = 100;
    std::vector<uint64_t> nums {};

    while (nums.size() < sampleSize) {
        nums.push_back(static_cast<uint64_t>(distribution(randomEngine)));
    }

    std::cout << "Origin: ";
    printVector(nums);

    auto heapPtr = std::make_unique<Heap<uint64_t>>(
            std::vector<uint64_t> (nums),
            [](const uint64_t& a, const uint64_t& b) { return a <= b; }
    );

    std::cout << "Sorted: ";
    auto sortedNums = std::vector<uint64_t> {};
    while (!heapPtr->empty()) {
        sortedNums.push_back(heapPtr->top());
        heapPtr->pop();
    }

    printVector(sortedNums);

    std::cout << "Verified: " << verifyTwoNumberListIdentical(nums, sortedNums);

    return 0;
}
