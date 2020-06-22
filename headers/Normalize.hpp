#pragma once

#include <tuple>
#include "Board.hpp"

static constexpr int normalize(const int x) {
    return 8 * sizeof(int) - __builtin_clz(x) - 1;
}
