#pragma once

#include <vector>
#include "NTuple.hpp"
#include <memory>

class Ensemble : public Model {
    std::vector<std::unique_ptr<NTuple>> tuples;

    double apply(const Board &board) const override;

    void update(const Board &, double) override;

public:
    Ensemble();

    ~Ensemble() override = default;
};