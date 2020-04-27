#include <iostream>
#include <memory>
#include "../headers/Ensemble.hpp"
#include "../headers/TD.hpp"
#include "../headers/ScoreWriter.hpp"

#ifdef _OPENMP

#include <omp.h>

#endif

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
#ifdef _OPENMP
    std::cout << "Max threads: " << omp_get_max_threads() << std::endl;
#endif
    // Initialize the model
    std::unique_ptr<Model> model = std::make_unique<Ensemble>(learning_rate);
    ScoreWriter scoreWriter(filename);

    // Train the model

#pragma omp parallel default(none) shared(epochs, model, scoreWriter, std::cout)
    {
#ifdef _OPENMP
#pragma omp single
        {
            std::cout << "Using " << omp_get_num_threads() << " threads" << std::endl;
        }
#endif
#pragma omp for schedule(dynamic, 100) nowait
        for (int epoch = 1; epoch <= epochs; epoch++) {
            auto[board, score] = playGame(model.get());

            const int max = *std::max_element(board.cbegin(), board.cend());

            scoreWriter.log(score, max);

            if (epoch % 100 == 0) {
                std::cout << " " << epoch << " / " << epochs;
#ifdef _OPENMP
                std::cout << " | Thread " << omp_get_thread_num();
#endif
                std::cout << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}
