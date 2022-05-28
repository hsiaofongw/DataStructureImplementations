#include <iostream>
#include <random>
#include "heap.hpp"

int main() {
    std::random_device randomDevice;  //Will be used to obtain a seed for the random number engine
    std::mt19937 randomEngine(randomDevice()); //Standard mersenne_twister_engine seeded with randomDevice()
    std::uniform_int_distribution<> distribution(1, 60);

    constexpr size_t sampleSize = 300;
    std::vector<uint64_t> nums {};

    while (nums.size() < sampleSize) {
        nums.push_back(static_cast<uint64_t>(distribution(randomEngine)));
    }

    std::cout << "Origin: ";
    printVector(nums);

    auto heapPtr = std::make_unique<Heap<uint64_t>>(std::move(nums));

    std::cout << "Sorted: ";
    auto sortedNums = std::vector<uint64_t> {};
    while (!heapPtr->empty()) {
        sortedNums.push_back(heapPtr->top());
        heapPtr->pop();
    }

    printVector(sortedNums);

    return 0;
}
