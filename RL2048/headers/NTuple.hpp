#pragma once

#include <functional>

#include "Board.hpp"
#include "Model.hpp"

using TupleView = std::function<int(const Board &, int)>;

class NTuple : Model {
    int n, m;
    double *LUT;

    TupleView view;

public:
    NTuple(int n, int m, TupleView func);

    ~NTuple() override;

    int address(const Board &) const;

    double apply(const Board &) const override;

    void update(const Board &, double) override;
};
