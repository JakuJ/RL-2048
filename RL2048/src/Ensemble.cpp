#include "../headers/Ensemble.hpp"
#include "../headers/Board.hpp"

Ensemble::Ensemble() {
    static constexpr int n = 4, m = 13;

    for (int i = 0; i < 4; i++) {
        tuples.push_back(std::make_unique<NTuple>(n, m, [i](const Board &board, int ix) -> int {
            return board.at(i, ix);
        }));

        tuples.push_back(std::make_unique<NTuple>(n, m, [i](const Board &board, int ix) -> int {
            return board.at(ix, i);
        }));
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tuples.push_back(std::make_unique<NTuple>(n, m, [i, j](const Board &board, int ix) -> int {
                return board.at(i + (ix / 2), j + (ix % 2));
            }));
        }
    }
}

double Ensemble::apply(const Board &board) const {
    double score = 0;

    for (const auto &tuple : tuples) {
        score += tuple->apply(board);
    }
    return score;
}

void Ensemble::update(const Board &board, double expected) {
    double val = apply(board);

    double error = expected - val;
    double delta = 0.0025 * error;

    for (auto &tuple : tuples) {
        tuple->update(board, delta);
    }
}
