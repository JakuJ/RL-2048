#pragma once

#include "Model.hpp"

// Dummy class allowing template polymorphism in the Ensemble class
struct NTupleInterface : public Model {
    ~NTupleInterface() override = default;

    void copyLUT(int stage){};
};