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

    for (int i = 0; i < Board::size; i++) {
        for (int j = 0; j < Board::size; j++) {
            int n2 = board.at(i, j);
            if (n2 >= 8) {
                min += (normalize(n2) - 2) * n2;
            }
            if (n2 >= 4) {
                max += (normalize(n2) - 1) * n2;
            }
        }
    }

    return std::make_tuple(min, max);
}
