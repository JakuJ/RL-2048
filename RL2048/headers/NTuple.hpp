#pragma once

#include <functional>
#include <tuple>
#include <cmath>
#include <memory>
#include <iostream>

#include "Board.hpp"
#include "Model.hpp"
#include "Normalize.hpp"
#include "NTupleInterface.hpp"

class SymmetryExpander;

template<int N>
class NTuple : public NTupleInterface {
    std::shared_ptr<double> LUT;

    std::tuple<int, int> indices[N];

    int powers[N]{0};

    [[nodiscard]] int address(const Board &) const;

    NTuple(int, std::tuple<int, int>[N], std::shared_ptr<double>);

public:
    NTuple(int, std::tuple<int, int>[N]);

    NTuple(const NTuple &) = delete;

    NTuple(NTuple &&) = delete;

    ~NTuple() override = default;

    [[nodiscard]] double apply(const Board &) const override;

    void update(const Board &, double) override;

    friend class SymmetryExpander;
};

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N]) {
    size_t size = static_cast<size_t>(std::pow(m, N));
    LUT = std::shared_ptr<double>(new double[size], std::default_delete<double[]>());

    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N], std::shared_ptr<double> weights): LUT(weights) {
    std::cout << "Creating a tuple from shared weight array at " << weights.get() << std::endl;
    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
int NTuple<N>::address(const Board &board) const {
    int address = 0;
    for (int i = 0; i < N; i++) {
        auto[x, y] = indices[i];
        address += normalize(board.at(x, y)) * powers[i];
    }
    return address;
}

template<int N>
double NTuple<N>::apply(const Board &board) const {
    return LUT.get()[address(board)];
}

template<int N>
void NTuple<N>::update(const Board &board, double delta) {
    int index = address(board);

#pragma omp atomic
    LUT.get()[index] += delta;
}
