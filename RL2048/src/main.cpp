#include <cassert>
#include <iostream>
#include <vector>
#include "../headers/Ensemble.hpp"
#include "../headers/TD.hpp"
#include "../headers/helpers.hpp"

int main() {
    std::unique_ptr<Model> model = std::make_unique<Ensemble>();

    const int epochs = 10000;
    for (int epoch = 0; epoch < epochs; epoch++) {
        auto[board, score] = playGame(model.get());

        auto[lower_bound, upper_bound] = score_bounds(board);
        assert(score >= lower_bound && score <= upper_bound);

        if (board.maximum() >= 2048) {
            std::cout << "Epoch " << epoch << " / " << epochs << ", score: " << score << std::endl;
            std::cout << board << std::endl;
            break;
        }
    }

    return 0;
}
