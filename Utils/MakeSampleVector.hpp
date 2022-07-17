//
// Created by 韦晓枫 on 2022/7/17.
//

#ifndef DATASTRUCTUREIMPLEMENTATIONS_MAKESAMPLEVECTOR_HPP
#define DATASTRUCTUREIMPLEMENTATIONS_MAKESAMPLEVECTOR_HPP

#include <memory>
#include <random>

namespace Utils {

    std::unique_ptr<std::vector<bool>> makeSampleVector(size_t population) {
        auto randomDevice = std::random_device {};
        auto randomEngine = std::default_random_engine { randomDevice() };
        auto distribution = std::bernoulli_distribution {};
        auto sampleVectorPtr = std::make_unique<std::vector<bool>>();
        for (size_t i = 0; i < population; ++i) {
            auto sampleBit = static_cast<bool>(distribution(randomEngine));
            sampleVectorPtr->push_back(sampleBit);
        }

        return sampleVectorPtr;
    }

}

#endif //DATASTRUCTUREIMPLEMENTATIONS_MAKESAMPLEVECTOR_HPP
