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
    if (argc != 5) {
        std::cout << "Usage: ./RL2048 epochs learning_rate log_filename save_model_prefix" << std::endl;
        return EXIT_FAILURE;
    }

    int epochs = static_cast<int>(std::strtol(argv[1], nullptr, 10));
    auto learning_rate = std::stod(argv[2]);
    char *filename = argv[3];
    auto save_prefix = std::string(argv[4]);

    // Probe OpenMP
    std::cout << "Available threads: " << omp_get_max_threads() << std::endl;

    // Initialize the model
    std::unique_ptr<Model> model = std::make_unique<Ensemble>(learning_rate);
    ScoreWriter scoreWriter(filename);

    // Train the model
    int total = 0;

#pragma omp parallel default(none) shared(epochs, model, scoreWriter, std::cout, total, save_prefix)
    {
#pragma omp single
        {
            std::cout << "Using " << omp_get_num_threads() << " threads" << std::endl;
        }

#pragma omp for schedule(dynamic) nowait
        for (int epoch = 1; epoch <= epochs; epoch++) {
            const auto&&[board, score] = playGame(model.get());

            const int max = *std::max_element(board.cbegin(), board.cend());
            const int score2 = score; // hack

#pragma omp critical
            {
                scoreWriter.log(score2, max);
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
                    case 16384:
                        std::cout << std::endl << RED << "** 16K **" << RESET << std::endl;
                        break;
                    default:
                        std::cout << " ";
                        break;
                }
            }

#pragma omp atomic
            total++;

            if (total % 100 == 0) {
                std::cout << " " << total << " / " << epochs << std::endl;
            }

            // Unsafe
            if (total % 50000 == 0) {
                model->save_model(save_prefix + std::to_string(total / 1000) + "k_");
            }
        }
    }

    return EXIT_SUCCESS;
}
