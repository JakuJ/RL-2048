#pragma once

#include <vector>
#include <memory>
#include <string>
#include "NTuple.hpp"

class Ensemble : public Model {
    const double learning_rate;

    std::vector<std::unique_ptr<NTupleInterface>> tuples;

    [[nodiscard]] double apply(const Board &board) const override;

    void update(const Board &, double) override;

    void save_model(std::string path) override;

    void load_model(std::string path) override;

public:
    explicit Ensemble(double learning_rate);

    ~Ensemble() override = default;
};
