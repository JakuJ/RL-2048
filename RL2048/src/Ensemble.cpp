#include "../headers/Ensemble.hpp"
#include "../headers/SymmetryExpander.hpp"

#include <numeric>

Ensemble::Ensemble(double lr) : learning_rate(lr) {
    constexpr int m = 15;
    tuples.reserve(17);

    for (int i = 0; i < Board::size; ++i) {
        std::tuple<int, int> row[4] = {{0, i},
                                       {1, i},
                                       {2, i},
                                       {3, i}};

        std::tuple<int, int> column[4] = {{i, 0},
                                          {i, 1},
                                          {i, 2},
                                          {i, 3}};

        tuples.emplace_back(new NTuple<4>{m, row});
        tuples.emplace_back(new NTuple<4>{m, column});
    }

    for (int i = 0; i < Board::size - 1; ++i) {
        for (int j = 0; j < Board::size - 1; ++j) {
            std::tuple<int, int> square[4] = {
                    {i,     j},
                    {i + 1, j},
                    {i + 1, j + 1},
                    {i,     j + 1},
            };

            tuples.emplace_back(new NTuple<4>{m, square});
        }
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::tuple<int, int> rectangle[6]{{i,     j},
                                              {i,     j + 1},
                                              {i + 1, j},
                                              {i + 1, j + 1},
                                              {i + 2, j},
                                              {i + 2, j + 1}};

            tuples.emplace_back(new NTuple<6>{m, rectangle});
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            std::tuple<int, int> rectangle[6]{{i,     j},
                                              {i + 1, j},
                                              {i,     j + 1},
                                              {i + 1, j + 1},
                                              {i,     j + 2},
                                              {i + 1, j + 2}};

            tuples.emplace_back(new NTuple<6>{m, rectangle});
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::tuple<int, int> mini_col[2]{{i,     j},
                                             {i + 1, j}};
            std::tuple<int, int> mini_row[2]{{j, i},
                                             {j, i + 1}};

            tuples.emplace_back(new NTuple<2>{m, mini_col});
            tuples.emplace_back(new NTuple<2>{m, mini_row});
        }
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
