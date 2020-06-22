#pragma once

#include <fstream>

class ScoreWriter {
    std::ofstream outfile;

public:
    explicit ScoreWriter(const char *filename);

    void log(int score, int maxTile);
};
