#include "../headers/Ensemble.hpp"
#include "../headers/Board.hpp"
#include "../headers/SymmetryExpander.hpp"

Ensemble::Ensemble(double lr) : learning_rate(lr) {
    static constexpr int m = 13;
    tuples.reserve(16);

    std::tuple<int, int> outer[7]{{0, 0},
                                  {1, 0},
                                  {2, 0},
                                  {3, 0},
                                  {2, 1},
                                  {3, 1},
                                  {3, 2}};

    for (auto *k : SymmetryExpander::expand<7>(m, outer)) {
        tuples.emplace_back(k);
    }

    std::tuple<int, int> inner[7]{{0, 1},
                                  {1, 1},
                                  {2, 1},
                                  {3, 1},
                                  {2, 2},
                                  {3, 2},
                                  {3, 3}};

    for (auto *k : SymmetryExpander::expand<7>(m, inner)) {
        tuples.emplace_back(k);
    }
}

double Ensemble::apply(const Board &board) const {
    double score = 0;

    for (auto &tuple : tuples) {
        score += tuple->apply(board);
    }
    return score;
}

void Ensemble::update(const Board &board, double error) {
    double delta = learning_rate * error;

    for (auto &tuple : tuples) {
        tuple->update(board, delta);
    }
}
