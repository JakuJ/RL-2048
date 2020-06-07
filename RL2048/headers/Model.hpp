#pragma once

#include <string>
#include "Board.hpp"

struct Model {
    virtual ~Model() = default;

    virtual void update(const Board &, double) = 0;

    virtual void save_model(const std::string &) const {}

    virtual void load_model(const std::string &) {}

    [[nodiscard]] virtual double apply(const Board &) const = 0;
};
