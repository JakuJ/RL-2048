#pragma once

#include <random>

template<typename T>
T random(T min, T max) {
    static std::mt19937 gen{std::random_device{}()};
    return std::uniform_int_distribution<T>{min, max}(gen);
}
