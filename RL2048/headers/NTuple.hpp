#pragma once

#include <functional>
#include <tuple>
#include <cmath>
#include <memory>

#include "Board.hpp"
#include "Model.hpp"
#include "Normalize.hpp"
#include "NTupleInterface.hpp"

template<int N>
class NTuple : public NTupleInterface {  
    double **LUT;

    std::tuple<int, int> indices[N];

    int powers[N]{0};

    [[nodiscard]] int address(const Board &) const;

    int m = 15;

public:
    bool usedLUT[4]{ false,false,false,false };

    NTuple(int, std::tuple<int, int>[N]);

    NTuple(int, std::tuple<int, int>[N], std::shared_ptr<double>);

    NTuple(const NTuple &) = delete;

    NTuple(NTuple &&) = delete;

    ~NTuple() override = default;

    [[nodiscard]] double apply(const Board &) const override;

    void update(const Board &, double) override;

	void copyLUT(int stage);
};

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N]) {
    this->m = m;
    LUT = new double*[4];
	for (int i = 0; i < 4; i++)
	{
		LUT[i] = new double[static_cast<int>(std::pow(m, N))]{ 0 };
	}
    
	usedLUT[0] = true;
    for (int i = 0; i < N; i++) {
        indices[i] = ixs[i];
        powers[i] = std::pow(m, i);
    }
}

template<int N>
NTuple<N>::NTuple(int m, std::tuple<int, int> ixs[N], std::shared_ptr<double> weights): LUT(std::move(weights)) {
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
    return LUT[board.stage][address(board)];
}

template<int N>
void NTuple<N>::update(const Board &board, double delta) {
    int index = address(board);

#pragma omp atomic
    LUT[board.stage][index] += delta;
}

template<int N>
void NTuple<N>::copyLUT(int stage) {
    if(usedLUT[stage]){
            return;
    }
    usedLUT[stage] = true;
	for (int i = 0; i < static_cast<int>(std::pow(m, N)); i++)
	{
		LUT[stage][i] = LUT[stage - 1][i];
	}
}