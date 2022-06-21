#include <iostream>
#include <random>
#include <unordered_map>
#include <map>
#include "DataStructures/RedBlackTree.hpp"

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

    using SimpleKeyType = std::string;
    using SimpleValueType = uint64_t;
    using SimpleRedBlackNode = RedBlackNode<SimpleKeyType, SimpleValueType>;
    using SimpleRedBlackNodePtr = std::shared_ptr<SimpleRedBlackNode>;
    using SimpleRedBlackHandle = RedBlackTreeHandle<SimpleKeyType, SimpleValueType>;

    auto root = SimpleRedBlackNodePtr {};
    for (const auto& pair : testData) {
        auto keyPtr = std::make_shared<SimpleKeyType>(pair.first);
        auto valuePtr = std::make_shared<SimpleValueType>(pair.second);
        std::cout << "Insert: (" << pair.first << ", " << pair.second << ")\n";
        root = SimpleRedBlackHandle::insert(root, keyPtr, valuePtr);
        SimpleRedBlackHandle::debugPrintTreeExpr(root);
        std::cout << "\n";
    }

    SimpleRedBlackHandle::debugCheckDefinition(root);

    return 0;
}
