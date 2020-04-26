#pragma once

#include "Board.hpp"

class Model {
public:
    virtual ~Model() {}

    virtual double apply(const Board &) const = 0;

    virtual void update(const Board &, double) = 0;
};
