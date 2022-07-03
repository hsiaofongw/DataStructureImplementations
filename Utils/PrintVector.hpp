//
// Created by 韦晓枫 on 2022/7/1.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_PRINTVECTOR_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_PRINTVECTOR_HPP

#include <vector>
#include <iostream>

namespace Utils {

    using std::vector;
    using std::cout;

    template <typename T>
    void printVector(const vector<T> &elements) {
        cout << "[";
        size_t idx = 0;
        for (const auto &element : elements) {
            cout << element;
            if (idx != elements.size()-1) {
                cout << ", ";
            }
        }
        std::cout << "]";
    }
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_PRINTVECTOR_HPP
