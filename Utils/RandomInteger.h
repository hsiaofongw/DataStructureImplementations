//
// Created by 韦晓枫 on 2022/7/23.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_RANDOMINTEGER_H
#define DATASTRUCTUREIMPLEMENTATIONS_RANDOMINTEGER_H

#include <random>
#include <memory>

namespace Utils {

    template <typename IntType>
    class RandomIntegerGenerator {

    public:
        RandomIntegerGenerator(IntType a, IntType b) : dev(), enginePtr(), dist(a, b) {
            this->enginePtr = std::make_unique<std::default_random_engine>(this->dev());
        }

        IntType get() {
            return this->dist(*(this->enginePtr));
        }

    private:
        std::random_device dev;
        std::unique_ptr<std::default_random_engine> enginePtr;
        std::uniform_int_distribution<IntType> dist;
    };

}

#endif //DATASTRUCTUREIMPLEMENTATIONS_RANDOMINTEGER_H
