#include <iostream>
#include "SystemDesign/LFUCache.hpp"
#include <utility>

/**
 * ["LFUCache","put","put","get","put","get","get","put","get","get","get"]
   [[2],[1,1],[2,2],[1],[3,3],[2],[3],[4,4],[1],[3],[4]]
 * @return
 */

int main() {
    using LFUCache = SystemDesign::Cache::LFUCacheGen<>;
    LFUCache lfuCache (2);
    auto put = [&lfuCache](int key) -> void {
        std::cout << "put: " << key << std::endl;
        lfuCache.put(key, key);
        lfuCache.printLink();
    };
    auto get = [&lfuCache](int key) -> void {
        int val = lfuCache.get(key);
        std::cout << "get " << key << ": " << val << std::endl;
        lfuCache.printLink();
    };
    put(1);
    put(2);
    get(1);
    put(3);
    get(2);
    get(3);
    put(4);
    get(1);
    get(3);
    get(4);

    return 0;
}
