#pragma once

#include "Board.hpp"
#include "Model.hpp"

void learn(const Model *m, const Board &, const Board &);

std::tuple<Board, int> playGame(Model *m);