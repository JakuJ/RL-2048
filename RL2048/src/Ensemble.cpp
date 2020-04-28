#include "../headers/Ensemble.hpp"
#include "../headers/Board.hpp"

Ensemble::Ensemble(double lr) : learning_rate(lr) {
    static constexpr int m = 13;
    tuples.reserve(8);

    for (int i = 0; i < 4; i++) {
        std::tuple<int, int> rows[4]{{i, 0},
                                     {i, 1},
                                     {i, 2},
                                     {i, 3}};

        tuples.emplace_back(new NTuple<4>(m, rows));

        std::tuple<int, int> columns[4]{{0, i},
                                        {1, i},
                                        {2, i},
                                        {3, i}};

        tuples.emplace_back(new NTuple<4>(m, columns));
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::tuple<int, int> square[4]{{i,     j},
                                           {i + 1, j},
                                           {i + 1, j + 1},
                                           {i,     j + 1}};

            tuples.emplace_back(new NTuple<4>(m, square));
        }
    }
}

double Ensemble::apply(const Board &board) const {
    double score = 0;

    for (auto &tuple : tuples) {
        score += tuple->apply(board);
    }
    return score;
}

void Ensemble::update(const Board &board, double expected) {
    double val = apply(board);

    double error = expected - val;
    double delta = learning_rate * error;

    for (auto &tuple : tuples) {
        tuple->update(board, delta);
    }
}
