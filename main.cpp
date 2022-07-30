#include <iostream>
#include <map>
#include <set>
#include "DataStructures/RedBlackTree.hpp"
#include "Algorithms/TrapRainWater.hpp"
#include "Utils/PrintVector.hpp"
#include "Algorithms/SubStringSearch.hpp"
#include "Algorithms/NQueens.hpp"
#include "Algorithms/MaximumRectangle.hpp"
#include "Algorithms/SpiralMatrix.hpp"
#include "Algorithms/LongestConsecutiveSequence.hpp"
#include "Algorithms/MergeIntervals.hpp"
#include "Algorithms/MinPathSum.hpp"
#include "Algorithms/MaximumRectangle.hpp"
#include "Algorithms/QuickSort.hpp"
#include <fstream>
#include <string>
#include "Algorithms/Dijkstra.hpp"
#include <algorithm>
#include "Utils/RandomInteger.h"
#include "Algorithms/MinEditDistance.hpp"
#include "Algorithms/DistinctSubsequences.hpp"
#include "Algorithms/CoinChange.hpp"
#include "Algorithms/WordBreak.hpp"
#include "Algorithms/Fibonacci.hpp"
#include "Utils/PrintTable.hpp"
#include "Algorithms/PerfectSquares.hpp"
#include <array>
#include "Algorithms/Subsets.hpp"
#include <algorithm>
#include "Algorithms/IsSubSequence.hpp"
#include "Algorithms/WordSearch.hpp"
#include "SystemDesign/MeetingScheduler.hpp"
#include "Algorithms/GasStation.hpp"
#include "Algorithms/MaximumSquare.hpp"
#include "Algorithms/SimplifyPath.hpp"
#include "SystemDesign/LRUCache.hpp"
#include "SystemDesign/LFUCache.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

int main() {

    {
        using SystemDesign::Cache::LFUCache;
        using nlohmann::json;
        std::ifstream case2_1_fs ("SampleData/LFUCache/case2_1.json");
        std::ifstream case2_2_fs ("SampleData/LFUCache/case2_2.json");

        json case2_1 = json::parse(case2_1_fs);
        json case2_2 = json::parse(case2_2_fs);

        auto begin = case2_2.begin();
        auto end = case2_2.end();
        if (begin != end) {
            json first = *begin;
            int capacity = first[0].get<int>();
            std::cout << "capacity: " << capacity << std::endl;

            LFUCache lfuCache (capacity);

            auto get = [&lfuCache](int key) -> void {
                std::cout << "get " << key << ": " << lfuCache.get(key) << std::endl;
                lfuCache.printLink();
            };

            auto put = [&lfuCache](int key) -> void {
                std::cout << "put: " << key << std::endl;
                lfuCache.put(key, key);
                lfuCache.printLink();
            };

            ++begin;
            for (auto it = begin; it != end; ++it) {
                std::cout << it->size() << std::endl;
                size_t size = it->size();
                json _key = (*it)[0];
                int key = _key.get<int>();
                if (size == 1) {
                    // get
                    get(key);
                } else if (size == 2) {
                    // put
                    put(key);
                } else {
                    // no op.
                }
            }
        }

        return 0;
    }
}
