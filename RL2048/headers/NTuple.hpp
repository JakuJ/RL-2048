#pragma once

#include <functional>
#include <tuple>
#include <cmath>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "Board.hpp"
#include "Model.hpp"
#include "Normalize.hpp"
#include "NTupleInterface.hpp"

template<int N>
class NTuple : public NTupleInterface {
    constexpr static int stages = 4;

    std::shared_ptr<double *> LUT;

    std::tuple<int, int> indices[N];

    int powers[N]{0};

    size_t size;

    int m = 15;

    [[nodiscard]] int address(const Board &) const;

public:
    bool usedLUT[stages]{false, false, false, false};

    NTuple(int, std::tuple<int, int>[N]);

    NTuple(int, std::tuple<int, int>[N], std::shared_ptr<double *>);

    NTuple(const NTuple &) = delete;

    NTuple(NTuple &&) = delete;

    [[nodiscard]] double apply(const Board &) const override;

    void update(const Board &, double) override;

    void copyLUT(int stage) override;
};

template<int N>
NTuple<N>::NTuple(int t_m, std::tuple<int, int> ixs[N]) {
    m = t_m;
    LUT = std::shared_ptr<double *>(new double *[stages], std::default_delete<double *[]>());
    for (int i = 0; i < stages; i++) {
        LUT.get()[i] = new double[static_cast<int>(std::pow(m, N))]{0};
    }

    usedLUT[0] = true;
    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N], std::shared_ptr<double *> weights): LUT(weights) {
    size = static_cast<size_t>(std::pow(m, N));

    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
int NTuple<N>::address(const Board &board) const {
    int address = 0;
    for (int i = 0; i < N; i++) {
        const auto&[x, y] = indices[i];
        address += normalize(board.at(x, y)) * powers[i];
    }
    return address;
}

template<int N>
double NTuple<N>::apply(const Board &board) const {
    return LUT.get()[board.stage][address(board)];
}

template<int N>
void NTuple<N>::update(const Board &board, double delta) {
    int index = address(board);
#pragma omp atomic
    LUT.get()[board.stage][index] += delta;
}

template<int N>
void NTuple<N>::copyLUT(int stage) {
    if (usedLUT[stage]) {
        return;
    }
    usedLUT[stage] = true;
    for (int i = 0; i < static_cast<int>(std::pow(m, N)); i++) {
        LUT.get()[stage][i] = LUT.get()[stage - 1][i];
    }
}
