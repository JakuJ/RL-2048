#include "../headers/ScoreWriter.hpp"

ScoreWriter::ScoreWriter(const char *filename) : outfile(filename, std::ofstream::out) {
    outfile << "score,max" << '\n';
}

void ScoreWriter::log(int score, int maxTile) {
    outfile << score << "," << maxTile << '\n';
}
