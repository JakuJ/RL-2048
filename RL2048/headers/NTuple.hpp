#pragma once

#include <functional>
#include <tuple>
#include <cmath>

#include "Board.hpp"
#include "Model.hpp"
#include "Normalize.hpp"
#include "NTupleInterface.hpp"


template<int N>
class NTuple : public NTupleInterface {
    double *LUT;

    std::tuple<int, int> indices[N];

    int powers[N]{0};

public:
    NTuple(int, std::tuple<int, int>[N]);

    NTuple(const NTuple &) = delete;

    NTuple(NTuple &&) = delete;

    ~NTuple() override;

    [[nodiscard]] int address(const Board &) const;

    [[nodiscard]] double apply(const Board &) const override;

    void update(const Board &, double) override;
};

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N]) {
    LUT = new double[static_cast<size_t>(std::pow(m, N))];

    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
NTuple<N>::~NTuple() {
    delete[] LUT;
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
    return LUT[address(board)];
}

template<int N>
void NTuple<N>::update(const Board &board, double delta) {
    int index = address(board);

#pragma omp atomic
    LUT[index] += delta;
}
