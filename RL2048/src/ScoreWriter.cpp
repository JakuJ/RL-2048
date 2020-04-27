#include "../headers/ScoreWriter.hpp"

ScoreWriter::ScoreWriter(const char *filename) {
    outfile = std::ofstream(filename, std::ofstream::out);
    outfile << "score,max" << std::endl;
}

ScoreWriter::~ScoreWriter() {
    outfile.close();
}

void ScoreWriter::log(const int score, const int maxTile) {
    outfile << score << "," << maxTile << std::endl;
}
