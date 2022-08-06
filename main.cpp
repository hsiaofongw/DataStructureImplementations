#include "DataStructures/RotatedSortedArray.hpp"
#include <iostream>

int main() {
    {
        using DataStructure::RotatedSortedArray::getTestCases;
        using DataStructure::RotatedSortedArray::findSingularPoint;
        using DataStructure::RotatedSortedArray::bisectSearchInRotatedSortedArray;

        std::vector<int> nums = { 1, 3, 5 };
        int target = 3;
        bisectSearchInRotatedSortedArray(nums, target);

        return 0;
    }
}
