#include <iostream>
#include <fstream>
#include "TestCases/MedianOfTwoSortedArrayTestCase.h"
#include <string>
#include "Utils/Parser/MyTestCaseParser.hpp"

int main() {
    auto parsedResult = Utils::Parser::MyTestCaseParser::parse(MEDIAN_OF_TWO_SORTED_ARRAY_TESTCASES_CONTENT);
    if (parsedResult) {
        for (auto &item : parsedResult.value()) {
            std::cout << "key: " << item.first << std::endl;
            std::cout << "nums: ";
            if (item.second.empty()) {
                std::cout << "<empty vector>";
            } else {
                for (auto &num : item.second) {
                    std::cout << num << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
