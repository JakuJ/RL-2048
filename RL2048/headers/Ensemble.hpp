#pragma once

#include <vector>
#include <memory>
#include <string>
#include "NTuple.hpp"

class Ensemble : public Model {
    double learning_rate;

    std::vector<std::unique_ptr<NTupleInterface>> tuples;

public:
    explicit Ensemble(double learning_rate);

    void update(const Board &, double) override;

    void save_model(const std::string &path) const override;

    ~Ensemble() override = default;

    void load_model(const std::string &path) override;


    [[nodiscard]] double apply(const Board &board) const override;

    void adapt_lr(int epoch) override;
};
