#include <iostream>
#include "SystemDesign/LFUCache.hpp"
#include <utility>

/**
 [[2],[2,1],[3,2],[3],[2],[4,3],[2],[3],[4]]
 */

int main() {
    using LFUCache = SystemDesign::Cache::LFUCacheGen<>;
    LFUCache lfuCache (2);
    lfuCache.printInternal();

    auto put = [&lfuCache](int key, int value) -> void {
        std::cout << "put: " << key << std::endl;
        lfuCache.put(key, key);
        lfuCache.printInternal();
    };
    auto get = [&lfuCache](int key) -> void {
        int val = lfuCache.get(key);
        std::cout << "get " << key << ": " << val << std::endl;
        lfuCache.printInternal();
    };

    put(2,1);
    put(3,2);
    get(3);
    get(2);
    put(4,3);
    get(2);
    get(3);
    get(4);

    return 0;
}
