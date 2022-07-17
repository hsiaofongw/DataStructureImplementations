//
// Created by 韦晓枫 on 2022/7/17.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MATRIX_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MATRIX_HPP

#include <vector>

namespace Interface {
    template <typename T = int>
    using Matrix = std::vector<std::vector<T>>;

    using IntMatrix = Matrix<int>;
}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MATRIX_HPP
