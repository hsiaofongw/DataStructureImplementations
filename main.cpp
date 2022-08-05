#include "Algorithms/FindLargestIndexInRotatedSortedArray.hpp"
#include <iostream>

int main() {
    {
        using Algorithm::RotatedArray::getTestCases;
        using Algorithm::RotatedArray::findSingularPoint;
        using Algorithm::RotatedArray::bisectSearchInRotatedSortedArray;

        std::vector<int> nums = { 1, 3, 5 };
        int target = 3;
        bisectSearchInRotatedSortedArray(nums, target);

        return 0;
    }
}
