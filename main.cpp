#include "Algorithms/LargestRectangleInHistogram.hpp"

int main() {
    {
        using Algorithm::LargestRectangleInHistogram::Solution;
        Solution solution;
        std::vector<int> testCase1 { 2,1,5,6,2,3 };
        auto ans = solution.largestRectangleArea(testCase1);
        std::cout << "ans: " << ans << std::endl;

        std::vector<int> testCase2 { 2,4 };
        ans = solution.largestRectangleArea(testCase2);
        std::cout << "ans: " << ans << std::endl;

        return 0;
    }
}
