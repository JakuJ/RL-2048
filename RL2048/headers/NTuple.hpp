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
    std::shared_ptr<double> LUT;

    std::tuple<int, int> indices[N];

    int powers[N]{0};

    size_t size;

    [[nodiscard]] int address(const Board &) const;

public:
    NTuple(int, std::tuple<int, int>[N]);

    NTuple(int, std::tuple<int, int>[N], std::shared_ptr<double>);

    NTuple(const NTuple &) = delete;

    NTuple(NTuple &&) = delete;

    ~NTuple() override = default;

    [[nodiscard]] double apply(const Board &) const override;

    void update(const Board &, double) override;

    void save_model(const std::string &path) const override;

    void load_model(const std::string &path) override;
};

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N]) {
    size = static_cast<size_t>(std::pow(m, N));

    auto weights = new double[size]{0};
    LUT = std::shared_ptr<double>(weights, std::default_delete<double[]>());

    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N], std::shared_ptr<double> weights): LUT(weights) {
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
    return LUT.get()[address(board)];
}

template<int N>
void NTuple<N>::update(const Board &board, double delta) {
    int index = address(board);

#pragma omp atomic
    LUT.get()[index] += delta;
}

template<int N>
void NTuple<N>::save_model(const std::string &path) const {
    std::ofstream out(path, std::ios::out | std::ios::binary);
    out.write(reinterpret_cast<char *>(LUT.get()), size * sizeof(double));
}

template<int N>
void NTuple<N>::load_model(const std::string &path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    in.read(reinterpret_cast<char *>(LUT.get()), size * sizeof(double));
}
