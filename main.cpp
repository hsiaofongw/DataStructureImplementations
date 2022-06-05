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

std::unique_ptr<std::vector<bool>> makeSampleVector(size_t population) {
    auto randomDevice = std::random_device {};
    auto randomEngine = std::default_random_engine { randomDevice() };
    auto distribution = std::bernoulli_distribution {};
    auto sampleVectorPtr = std::make_unique<std::vector<bool>>();
    for (size_t i = 0; i < population; ++i) {
        auto sampleBit = static_cast<bool>(distribution(randomEngine));
        sampleVectorPtr->push_back(sampleBit);
    }

    return sampleVectorPtr;
}

using Handle = BST::BSTHandle<std::string, uint64_t>;

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

    auto handlePtr = std::make_shared<Handle>();
    auto print = [](const decltype(handlePtr)& handlePtr) -> void {
        handlePtr->traverseInOrderLNR([](const auto& nodePtr) -> void {
            std::cout << "(" << (*nodePtr->keyPtr) << ", " << (*nodePtr->valuePtr) << ")\n";
        });
    };

    auto load = [](const decltype(testData)& data, decltype(handlePtr)& handlePtr) -> void {
        std::for_each(std::begin(data), std::end(data), [&handlePtr](auto pair) {
            std::cout << "insert: " << "(" << pair.first << ", " << pair.second << ")\n";
            auto strPtr = std::make_shared<std::string>(pair.first);
            auto valPtr = std::make_shared<uint64_t>(pair.second);
            handlePtr->insert(strPtr, valPtr);
        });
    };

    std::cout << "加载数据：\n";
    load(testData, handlePtr);

    std::cout << "打印：\n";
    print(handlePtr);

    auto keyVector = std::vector<std::string> {};
    for (const auto& pair : testData) {
        keyVector.emplace_back(pair.first);
    }

    auto sampleVectorPtr = makeSampleVector(keyVector.size());
    const auto& sampleVector = *sampleVectorPtr;

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

    std::cout << "打印：\n";
    print(handlePtr);

    std::cout << "单次 deleteMin 测试：\n";
    for (size_t idx = 0; idx < sampleVector.size(); ++idx) {
        std::cout << "deleteMin()\n";
        handlePtr->deleteMin();
        std::cout << "==\n";
        print(handlePtr);
        std::cout << "==\n";
    }

    std::cout << "加载数据：\n";
    load(testData, handlePtr);

    std::cout << "打印：\n";
    print(handlePtr);

    std::cout << "单次 deleteMax 测试：\n";
    for (size_t idx = 0; idx < sampleVector.size(); ++idx) {
        std::cout << "deleteMax()\n";
        handlePtr->deleteMax();
        std::cout << "==\n";
        print(handlePtr);
        std::cout << "==\n";
    }

    std::cout << "加载数据：\n";
    load(testData, handlePtr);

    std::cout << "打印：\n";
    print(handlePtr);

    std::cout << "随机删除测试：\n";
    for (size_t idx = 0; idx < sampleVector.size(); ++idx) {
        auto sampleBit = static_cast<bool>(sampleVector[idx]);
        if (sampleBit) {
            const auto& key = keyVector[idx];
            std::cout << "deleteKey(" << key << ")\n";
            handlePtr->deleteKey(key);
            std::cout << "==\n";
            print(handlePtr);
            std::cout << "==\n";
        }
    }

    std::cout << "加载数据：\n";
    load(testData, handlePtr);

    std::cout << "打印：\n";
    print(handlePtr);

    std::cout << "全量删除测试：\n";
    for (const auto& key : keyVector) {
        std::cout << "deleteKey(" << key << ")\n";
        handlePtr->deleteKey(key);
        std::cout << "==\n";
        print(handlePtr);
        std::cout << "==\n";
    }

    std::cout << "加载数据：\n";
    load(testData, handlePtr);

    std::cout << "打印：\n";
    print(handlePtr);

    std::cout << "随机范围 slicing 操作测试：\n";
    struct SliceSpec {
        size_t offset;
        size_t limit;
    };
    auto getRange = [](size_t keySize) -> SliceSpec {
        auto range = SliceSpec {};
        auto randomDev = std::random_device {};
        auto randomNg = std::default_random_engine { randomDev() };
        auto offsetDistrib = std::uniform_int_distribution<size_t> {0, keySize - 1 };
        auto offset = static_cast<size_t>(offsetDistrib(randomNg));
        auto lengthDistrib = std::uniform_int_distribution<size_t> { 1, keySize - offset };
        auto limit = static_cast<size_t>(lengthDistrib(randomNg));
        range.offset = offset;
        range.limit = limit;

        return range;
    };
    uint8_t times = 10;
    std::sort(std::begin(keyVector), std::end(keyVector));
    std::cout << "keyVector: ";
    std::for_each(std::begin(keyVector), std::end(keyVector), [](const auto& element) -> void {
        std::cout << element << " ";
    });
    std::cout << "\n";
    for (decltype(times) i = 0; i < times; ++i) {
        auto range = getRange(keyVector.size());

        auto beginKey = keyVector[range.offset];
        auto endKey = keyVector[range.offset + range.limit - 1];
        std::cout
            << "offset: " << range.offset
            << ", limit: " << range.limit
            << ", begin: " << beginKey
            << ", end: " << endKey;

        std::cout << " nodes: ";

        auto resultVectorPtr = handlePtr->rangeSearchMany(beginKey, endKey);
        const auto& resultVector = *resultVectorPtr;
        for (const auto& elementRef : resultVector) {
            std::cout << "(" << (*elementRef->keyPtr) << ", " << (*elementRef->valuePtr) << ") ";
        }
        std::cout << "\n";
    }

    return 0;
}
