#pragma once

#include <string>
#include <fstream>

class ScoreSaver{
    std::string filename;
    std::ofstream outfile;
    bool first;
    
public:
    ScoreSaver(std::string fname);
    void append(const int score, const int maxTile);
    ~ScoreSaver();
};