#include <iostream>
#include "SystemDesign/LFUCache.hpp"

int main() {
    std::cout << "Capacity of Cache: ";
    size_t capacity;
    std::cin >> capacity;

    using SystemDesign::Cache::LFUCache;
    LFUCache<> cache (capacity);

    while (true) {
        std::cout << "Instruction: ";
        std::string command;
        std::cin >> command;

        if (command == "get") {
            int key;
            std::cin >> key;
            if (auto valueMaybe = cache.get(key)) {
                std::cout << "Value is: " << valueMaybe.value() << std::endl;
            } else {
                std::cout << "No such key." << std::endl;
            }
        } else if (command == "put") {
            int key;
            std::cin >> key;
            int value;
            std::cin >> value;
            if (!cache.put(key, value)) {
                std::cout << "Put failed, something goes wrong." << std::endl;
            }
            std::cout << "Done." << std::endl;
        } else if (command == "showInternalState" || command == "show") {
            cache.printInternal();
        } else if (command == "exit") {
            std::cout <<  "Exiting..." << std::endl;
            return 0;
        } else {
            std::cerr << "Unknown command, exiting..." << std::endl;
            return 1;
        }
    }
}
