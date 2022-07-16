//
// Created by 韦晓枫 on 2022/7/16.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_SPIRALMATRIX_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_SPIRALMATRIX_HPP

#include <cstdint>
#include <vector>
#include <stack>
#include <iostream>

namespace Algorithm {

    namespace SpiralMatrix {

        /**
         * Spiral 顺序遍历二级数组：
         *
         * 思路：设定一些 turningPoints, 在到达 turningPoint 时掉头（90 度相对于当前顺时针转向）。
         */
        
        using std::vector;
        
        class Solution {
        public:
            vector<int> spiralOrder(const vector<vector<int>>& matrix) {
                intptr_t h = matrix.size();
                intptr_t w = matrix[0].size();

                std::cout << "spiralOrder(w: " << w << ", h: " << h << ")\n";

                vector<int> trace;
                trace.reserve(w*h);

                doTraverseEdge(trace, matrix, 0, 0, w, h);
                return trace;
            }

        private:
            void doTraverseEdge(
                    vector<int>& trace,
                    const vector<vector<int>>& matrix,
                    intptr_t i,
                    intptr_t j,
                    intptr_t w,
                    intptr_t h
            ) {
                std::cout << "doTraverseEdge("
                    << "i: " << i << ", "
                    << "j: " << j << ")\n";

                if (w <= 0 || h <= 0) {
                    return;
                }

                std::stack<intptr_t> turningPoints;
                turningPoints.push(w-1+h-1+w-1);
                turningPoints.push(w-1+h-1);
                turningPoints.push(w-1);

                vector<vector<intptr_t>> directions = { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
                auto direction = directions.begin();
                intptr_t totalSteps = 0;
                if (w >= 2 && h >= 2) {
                    totalSteps = 2*w + 2*h - 4;
                } else if (w >= 2 || h >= 2) {
                    totalSteps = std::max(w, h);
                } else {
                    totalSteps = 1;
                }
                for (intptr_t step = 0; step < totalSteps; ++step) {
                    std::cout << "step: " << step << "\n";

                    trace.emplace_back(matrix[i][j]);

                    if (!turningPoints.empty() && step == turningPoints.top()) {
                        std::cout << "turn\n";
                        ++direction;
                        turningPoints.pop();
                    }

                    std::cout << "before: " << "i: " << i << ", j: " << j << "\n";

                    i += direction->at(0);
                    j += direction->at(1);

                    std::cout << "after: " << "i: " << i << ", j: " << j << "\n";
                }

                doTraverseEdge(trace, matrix, i+1, j+1, w-2, h-2);
            }
        };
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_SPIRALMATRIX_HPP
