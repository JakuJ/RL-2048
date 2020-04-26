#include <cmath>
#include "../headers/NTuple.hpp"
#include "../headers/helpers.hpp"

NTuple::NTuple(int n, int m, TupleView func) : n(n), m(m), view(std::move(func)) {
    LUT = new double[(int)std::pow(m, n)]{0};
}

NTuple::~NTuple() {
    delete[] LUT;
}

int NTuple::address(const Board &board) const {
    static const int powers[] = {1, m, m * m, m * m * m};

    int address = 0;
    for (int i = 0; i < n; i++) {
        address += normalize(view(board, i)) * powers[i];
    }
    return address;
}

double NTuple::apply(const Board &board) const {
    return LUT[address(board)];
}

void NTuple::update(const Board &board, double delta) {
    LUT[address(board)] += delta;
}
