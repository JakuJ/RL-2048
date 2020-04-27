#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "../headers/Ensemble.hpp"
#include "../headers/TD.hpp"
#include "../headers/helpers.hpp"
#include "../headers/ScoreSaver.hpp"

#define DEBUG 0

void handleCLIArgs(int, char*[], int&, double&, std::string&);

int main(int argc, char *argv[]) {
    if(argc != 4) throw std::invalid_argument("ivalid number of arguments");
    int epochs;
    double learning_rate;
    std::string filename;
    handleCLIArgs(argc, argv, epochs, learning_rate, filename);
    std::unique_ptr<Model> model = std::make_unique<Ensemble>(learning_rate);
    std::unique_ptr<ScoreSaver> scoreSaver = std::make_unique<ScoreSaver>(filename);

    for (int epoch = 1; epoch <= epochs; epoch++) {
        auto[board, score] = playGame(model.get());

#if DEBUG
        auto[lower_bound, upper_bound] = score_bounds(board);
        assert(score >= lower_bound && score <= upper_bound);
#endif
        const int max = *std::max_element(board.cbegin(), board.cend());

        scoreSaver->append(score, max);

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

void handleCLIArgs(int argc, char *argv[], int &epoch, double &learning_rate,
    std::string &filename){
    epoch = atoi(argv[1]);
    learning_rate = std::stod(std::string(argv[2]));
    filename = std::string(argv[3]);
}
