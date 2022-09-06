#include <iostream>
#include <fstream>

#include <string>
#include "Utils/Parser/MyTestCaseParser.hpp"
#include "TestCases/MedianOfTwoTestCases.hpp"

int main() {
    using TestCases::MedianOfTwoSortedArray::getTestCases;

    auto testCases = getTestCases();
    std::cout << "";

    return 0;
}
