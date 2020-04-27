#include "../headers/Ensemble.hpp"
#include "../headers/Board.hpp"

Ensemble::Ensemble(double lr) : learning_rate(lr) {
    static constexpr int n = 4, m = 13;

    tuples.reserve(17);

    for (int i = 0; i < 4; i++) {
        tuples.emplace_back(n, m, [i](const Board &board, int ix) -> int {
            return board.at(i, ix);
        });

        tuples.emplace_back(n, m, [i](const Board &board, int ix) -> int {
            return board.at(ix, i);
        });
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tuples.emplace_back(n, m, [i, j](const Board &board, int ix) -> int {
                return board.at(i + (ix / 2), j + (ix % 2));
            });
        }
    }
}

double Ensemble::apply(const Board &board) const {
    double score = 0;

    for (const auto &tuple : tuples) {
        score += tuple.apply(board);
    }
    return score;
}

void Ensemble::update(const Board &board, double expected) {
    double val = apply(board);

    double error = expected - val;
    double delta = learning_rate * error;

    for (auto &tuple : tuples) {
        tuple.update(board, delta);
    }
}
