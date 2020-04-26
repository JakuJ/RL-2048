#include <cassert>
#include <iostream>
#include <vector>
#include "../headers/Ensemble.hpp"
#include "../headers/TD.hpp"
#include "../headers/helpers.hpp"

#define DEBUG 0

int main() {
    std::unique_ptr<Model> model = std::make_unique<Ensemble>();

    const int epochs = 100000;
    for (int epoch = 1; epoch <= epochs; epoch++) {
        auto[board, score] = playGame(model.get());

#if DEBUG
        auto[lower_bound, upper_bound] = score_bounds(board);
        assert(score >= lower_bound && score <= upper_bound);
#endif
        const int max = *std::max_element(board.cbegin(), board.cend());

        static const std::string red("\033[0;31m");
        static const std::string green("\033[0;32m");
        static const std::string yellow("\033[0;93m");
        static const std::string reset("\033[0m");

        switch (max) {
            case 2048:
                std::cout << green << "|" << reset;
                break;
            case 4096:
                std::cout << yellow << "X" << reset;
                break;
            case 8192:
                std::cout << red << "@" << reset;
                break;
            default:
                std::cout << ".";
                break;
        }

        if (epoch % 100 == 0) {
            std::cout << " " << epoch << " / " << epochs << std::endl;
        }
    }

    return 0;
}
