#include <iostream>
#include <fstream>

#include <string>
#include "Utils/Parser/MyTestCaseParser.hpp"
#include "TestCases/MedianOfTwoTestCases.hpp"
#include "Algorithms/MedianOfTwoSortedArray.hpp"

int main() {
    /**
     * [1,2]
        [-1,3]
     */
    using Algorithm::MedianOfTwoSortedArray::medianOfTwoSortedArray;
    std::vector<int> lhs = { 1, 3 };
    std::vector<int> rhs = { 2 };
    auto median = medianOfTwoSortedArray(lhs, rhs);
    std::cout << "Ans: " << median << std::endl;
    return 0;
}
