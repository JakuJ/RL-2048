#pragma once

#include "Board.hpp"

struct Model {
    virtual ~Model() = default;

    virtual void update(const Board &, double) = 0;

    [[nodiscard]] virtual double apply(const Board &) const = 0;
};
