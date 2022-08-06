#include <iostream>
#include "SystemDesign/LFUCache.hpp"

int main() {
    using LFUCache = SystemDesign::Cache::LFUCacheGen<>;
    LFUCache lfuCache (3);
}
