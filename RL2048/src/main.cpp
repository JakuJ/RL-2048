#include <iostream>
#include <cassert>
#include "../headers/Ensemble.hpp"
#include "../headers/TD.hpp"
#include "../headers/ScoreWriter.hpp"

#define DEBUG 0

int main(int argc, char *argv[]) {
    // Handle CLI arguments
    if (argc != 4) {
        std::cout << "Usage: ./RL2048 epochs learning_rate log_filename" << std::endl;
        return EXIT_FAILURE;
    }

    size_t epochs = std::strtol(argv[1], nullptr, 10);
    auto learning_rate = std::stod(argv[2]);
    char *filename = argv[3];

    // Initialize the model
    std::unique_ptr<Model> model = std::make_unique<Ensemble>(learning_rate);
    ScoreWriter scoreWriter(filename);

    // Train the model
    for (int epoch = 1; epoch <= epochs; epoch++) {
        auto[board, score] = playGame(model.get());

#if DEBUG
        auto[lower_bound, upper_bound] = score_bounds(board);
        assert(score >= lower_bound && score <= upper_bound);
#endif
        const int max = *std::max_element(board.cbegin(), board.cend());

        scoreWriter.log(score, max);

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

    return EXIT_SUCCESS;
}
