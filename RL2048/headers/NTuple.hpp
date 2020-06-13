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

    constexpr static int stages = 16;

    double **LUT;

    std::tuple<int, int> indices[N];

    int powers[N]{0};

    size_t size;

    int m = 15;

    [[nodiscard]] int address(const Board &) const;

public:

	// teraz usedLUT jest dwuwymiarowa i z boolami, bo patrzymy po komorce
    bool **usedLUT;

    NTuple(int, std::tuple<int, int>[N]);

    NTuple(int, std::tuple<int, int>[N], double **);

    ~NTuple() override;

    NTuple(const NTuple &) = delete;

    NTuple(NTuple &&);

    [[nodiscard]] double apply(const Board &) const override;

    void update(const Board &, double) override;

    // void copyLUT(int stage) override;
};

template<int N>
NTuple<N>::NTuple(int t_m, std::tuple<int, int> ixs[N]) {
    m = t_m;
    LUT = new double *[stages];
	usedLUT = new bool *[stages];
    for (int i = 0; i < stages; i++) {
        LUT[i] = new double[static_cast<int>(std::pow(m, N))]{0};
		usedLUT[i] = new bool[static_cast<int>(std::pow(m, N))]{ false };
    }

    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N], double **weights): LUT(weights) {
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

// tutaj jest logika z przepisaniem wagi multi-stage
template<int N>
double NTuple<N>::apply(const Board &board) const {
	int index = address(board);
	if (usedLUT[board.stage][index] == false && board.stage > 0)
	{
	    // przepisanie wagi z poprzdniej tablicy
		usedLUT[board.stage][index] = true;
		LUT[board.stage][index] = LUT[board.stage - 1][index];
	}
    return LUT[board.stage][index];
}

template<int N>
void NTuple<N>::update(const Board &board, double delta) {
    int index = address(board);
#pragma omp atomic
    LUT[board.stage][index] += delta;
}

template<int N>
NTuple<N>::~NTuple() {
    for (int i = 0; i < stages; i++) {
        delete[] LUT[i];
    }
    delete[] LUT;
}

template<int N>
NTuple<N>::NTuple(NTuple &&rhs) {
    if (this != &rhs) {
        LUT = rhs.LUT;
        rhs.LUT = nullptr;

        size = rhs.size;
        m = rhs.m;

        std::copy(std::begin(rhs.powers), std::end(rhs.powers), powers);
        std::copy(std::begin(rhs.indices), std::end(rhs.indices), indices);
    }
}
