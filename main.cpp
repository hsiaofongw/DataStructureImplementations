#include "Algorithms/FindLargestIndexInRotatedSortedArray.hpp"
#include <iostream>

int main() {
    {
        using Algorithm::FindLargestIndexInRotatedSortedArray::getTestCases;
        using Algorithm::FindLargestIndexInRotatedSortedArray::findSingularPoint;

        auto testCases = getTestCases();
        for (size_t id = 0; id < testCases.size(); ++id) {
            std::cout << "testCase: " << id << std::endl;
            std::cout << "input: ";
            const auto &testCase = testCases[id];
            for (const auto &x : testCase.input) {
                std::cout << x << " ";
            }
            std::cout << std::endl;

            std::optional<size_t> result = findSingularPoint(testCase.input);
            if (result) {
                std::cout << "result: " << result.value() << std::endl;
            } else {
                std::cout << "result: {}" << std::endl;
            }

            if (testCase.expectedOut) {
                std::cout << "expected: " << testCase.expectedOut.value() << std::endl;
            } else {
                std::cout << "expected: {}" << std::endl;
            }

            assert((testCase.expectedOut == result));
        }

        return 0;
    }
}
