#pragma once

#include <tuple>

#include "Board.hpp"

int normalize(int x);

std::tuple<int, int> score_bounds(const Board &board);
