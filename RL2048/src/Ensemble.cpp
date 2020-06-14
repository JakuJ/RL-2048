#include "../headers/Ensemble.hpp"
#include "../headers/SymmetryExpander.hpp"

#include <numeric>

Ensemble::Ensemble(double lr) : learning_rate(lr) {
    constexpr int m = 15;
    tuples.reserve(17);

    std::tuple<int, int> shape[6] = {{1, 1},
                                     {1, 0},
                                     {0, 0},
                                     {0, 1},
                                     {0, 2},
                                     {0, 3}};

    for (auto &&t : SymmetryExpander::expand<6>(m, shape)) {
        tuples.push_back(std::move(t));
    }

    std::tuple<int, int> other_shape[6] = {{2, 1},
                                           {2, 0},
                                           {1, 0},
                                           {1, 1},
                                           {1, 2},
                                           {1, 3}};

    for (auto &&t : SymmetryExpander::expand<6>(m, other_shape)) {
        tuples.push_back(std::move(t));
    }
}

double Ensemble::apply(const Board &board) const {
    return std::accumulate(tuples.cbegin(), tuples.cend(), 0.0, [&board](double acc, const auto &tuple) {
        return acc + tuple->apply(board);
    });
}

void Ensemble::update(const Board &board, double error) {
    const double delta = learning_rate * error;

    std::for_each(tuples.begin(), tuples.end(), [&](auto &t) {
        t->update(board, delta);
    });
}

void Ensemble::save_model(const std::string &path) const {
    for (size_t i = 0; i < tuples.size(); i++) {
        tuples[i]->save_model(path + std::to_string(i));
    }
}

void Ensemble::load_model(const std::string &path) {
    for (size_t i = 0; i < tuples.size(); i++) {
        tuples[i]->load_model(path + std::to_string(i));
    }
}

void Ensemble::copyLUTs(int stage) {
    for (auto &tuple : tuples) {
        tuple->copyLUT(stage);
    }
}

void Ensemble::adapt_lr(int epoch) {
    if (epoch == 20000)
        learning_rate = 0.001;
}

