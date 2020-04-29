#include <omp.h>
#include <iostream>
#include <memory>
#include "../headers/Ensemble.hpp"
#include "../headers/TD.hpp"
#include "../headers/ScoreWriter.hpp"

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define RESET "\033[0;0m"

int main(int argc, char *argv[]) {
    // Handle CLI arguments
    if (argc != 4) {
        std::cout << "Usage: ./RL2048 epochs learning_rate log_filename" << std::endl;
        return EXIT_FAILURE;
    }

    int epochs = static_cast<int>(std::strtol(argv[1], nullptr, 10));
    auto learning_rate = std::stod(argv[2]);
    char *filename = argv[3];

    // Setup OpenMP
    std::cout << "Available threads: " << omp_get_max_threads() << std::endl;
    // Initialize the model
    std::unique_ptr<Model> model = std::make_unique<Ensemble>(learning_rate);
    ScoreWriter scoreWriter(filename);

    // Train the model
    int total = 0;

#pragma omp parallel default(none) shared(epochs, model, scoreWriter, std::cout, total)
    {
#pragma omp single
        {
            std::cout << "Using " << omp_get_num_threads() << " threads" << std::endl;
        }

#pragma omp for schedule(dynamic) nowait
        for (int epoch = 1; epoch <= epochs; epoch++) {
            auto[board, score] = playGame(model.get());

            const int max = *std::max_element(board.cbegin(), board.cend());

#pragma omp critical
            {
                switch (max) {
                    case 2048:
                        std::cout << GREEN << "█" << RESET;
                        break;
                    case 4096:
                        std::cout << BLUE << "█" << RESET;
                        break;
                    case 8192:
                        std::cout << RED << "█" << RESET;
                        break;
                    default:
                        std::cout << " ";
                        break;
                }
                scoreWriter.log(score, max);
            }

#pragma omp atomic
            total++;

            if (total % 100 == 0) {
                std::cout << " " << total << " / " << epochs << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}
