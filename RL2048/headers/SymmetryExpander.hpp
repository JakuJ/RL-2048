#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>
#include <array>
#include "NTuple.hpp"
#include "NTupleInterface.hpp"

class SymmetryExpander {
    static constexpr int numSymmetries = 8;

    static std::vector<std::tuple<int, int>> getSymmetries(std::tuple<int, int> position);

public:
    template<int N>
    static std::vector<std::unique_ptr<NTupleInterface>> expand(int, std::tuple<int, int>[N]);
};

std::vector<std::tuple<int, int>> SymmetryExpander::getSymmetries(std::tuple<int, int> position) {
    const auto[r, c] = position;
    constexpr int M = Board::size - 1;

    return std::vector<std::tuple<int, int>>{
            {c,     r},
            {c,     M - r},
            {M - c, r},
            {M - c, M - r},
            {r,     c},
            {r,     M - c},
            {M - r, c},
            {M - r, M - c}
    };
}

template<int N>
std::vector<std::unique_ptr<NTupleInterface>> SymmetryExpander::expand(int m, std::tuple<int, int> indices[N]) {
    std::vector<std::array<std::tuple<int, int>, N>> symIndices(numSymmetries);

    for (int i = 0; i < N; i++) {
        auto syms = getSymmetries(indices[i]);

        for (int s = 0; s < numSymmetries; s++) {
            symIndices[s][i] = syms[s];
        }
    }

    std::vector<std::unique_ptr<NTupleInterface>> ret;
    ret.reserve(numSymmetries);

    const auto size = static_cast<size_t>(std::pow(m, N));

    const int stages = NTuple<1>::stages;

    auto LUT = new double *[stages];
    for (int i = 0; i < stages; i++) {
        LUT[i] = new double[size]{0};
    }

    auto usedLUT = new bool *[stages];
    for (int i = 0; i < stages; i++) {
        usedLUT[i] = new bool[size]{false};
    }

    for (auto &&ixs : symIndices) {
        ret.emplace_back(new NTuple<N>(m, ixs.begin(), LUT, usedLUT));
    }
    return ret;
}

