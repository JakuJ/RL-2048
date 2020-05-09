#include "../headers/Ensemble.hpp"
#include "../headers/Board.hpp"

Ensemble::Ensemble(double lr) : learning_rate(lr) {
    static constexpr int m = 15;
    tuples.reserve(17);

    for (int i : {0, 1, 2, 3}) {
        std::tuple<int, int> row[4] = {{0, i},
                                       {1, i},
                                       {2, i},
                                       {3, i}};

        std::tuple<int, int> column[4] = {{i, 0},
                                          {i, 1},
                                          {i, 2},
                                          {i, 3}};

        tuples.emplace_back(new NTuple<4>(m, row));
        tuples.emplace_back(new NTuple<4>(m, column));
    }

    for (int i = 0; i < Board::size - 1; i++) {
        for (int j = 0; j < Board::size - 1; j++) {
            std::tuple<int, int> square[4] = {
                    {i,     j},
                    {i + 1, j},
                    {i + 1, j + 1},
                    {i,     j + 1},
            };

            tuples.emplace_back(new NTuple<4>(m, square));
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

void Ensemble::update(const Board &board, double error) {
    const double delta = learning_rate * error;

    for (auto &tuple : tuples) {
        tuple->update(board, delta);
    }
}

void Ensemble::save_model(const std::string path) {
    for (size_t i = 0; i < tuples.size(); i++) {
        tuples[i]->save_model(path + std::to_string(i));
    }
}

void Ensemble::load_model(const std::string path) {
    for (size_t i = 0; i < tuples.size(); i++) {
        tuples[i]->load_model(path + std::to_string(i));
    }
}
