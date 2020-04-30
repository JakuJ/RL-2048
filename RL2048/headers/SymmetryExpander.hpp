#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>
#include "NTuple.hpp"
#include "NTupleInterface.hpp"

class SymmetryExpander {
    static const int numSymmetries = 8;

    static std::vector<std::tuple<int, int>> getSymmetries(std::tuple<int, int> position);

public:
    template<int N>
    static std::vector<NTupleInterface *> expand(int, std::tuple<int, int>[N]);
};

std::vector<std::tuple<int, int>> SymmetryExpander::getSymmetries(std::tuple<int, int> position) {
    const auto[r, c] = position;
    const int M = Board::size - 1;

    return std::vector<std::tuple<int, int>>{
            std::make_tuple(c, r),
            std::make_tuple(c, M - r),
            std::make_tuple(M - c, r),
            std::make_tuple(M - c, M - r),
            std::make_tuple(r, c),
            std::make_tuple(r, M - c),
            std::make_tuple(M - r, c),
            std::make_tuple(M - r, M - c)
    };
}

template<int N>
std::vector<NTupleInterface *> SymmetryExpander::expand(int m, std::tuple<int, int> indices[N]) {
    std::vector<std::array<std::tuple<int, int>, N>> symIndices(numSymmetries);

    for (int i = 0; i < N; i++) {
        auto syms = getSymmetries(indices[i]);

        for (int s = 0; s < numSymmetries; s++) {
            symIndices[s][i] = syms[s];
        }
    }

    std::vector < NTupleInterface * > ret;
    ret.reserve(numSymmetries);

    auto size = static_cast<size_t>(std::pow(m, N));

    auto weights = new double[size]{0};
    auto LUT = std::shared_ptr<double>(weights, std::default_delete<double[]>());

    for (auto ixs : symIndices) {
        ret.push_back(new NTuple<N>(m, ixs.begin(), LUT));
    }
    return ret;
}

