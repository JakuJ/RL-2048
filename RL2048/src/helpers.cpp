#include "../headers/helpers.hpp"

static inline unsigned int fast_log2(const unsigned int x) {
    unsigned int y;
    asm ( "\tbsr %1, %0\n"
    : "=r"(y)
    : "r" (x)
    );
    return y;
}

int normalize(int x) {
    return x ? static_cast<int>(fast_log2(x)) : 0;
}

std::tuple<int, int> score_bounds(const Board &board) {
    int min = 0, max = 0;

    for (const int *iter = board.cbegin(); iter != board.cend(); iter++) {
        const int field = *iter;

        if (field >= 8) {
            min += (normalize(field) - 2) * field;
        }
        if (field >= 4) {
            max += (normalize(field) - 1) * field;
        }
    }

    return std::make_tuple(min, max);
}
