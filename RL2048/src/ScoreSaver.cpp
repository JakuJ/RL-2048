#include "../headers/ScoreSaver.hpp"

ScoreSaver::ScoreSaver(std::string fname){
    outfile = std::ofstream(fname, std::ofstream::out);
    outfile << "[";
    first = true;
}

void ScoreSaver::append(const int score, const int maxTile){
    if(first){
        first = false;
    }else
    {
        outfile << ",";
    }
    
    outfile << "[" << score <<"," << maxTile << "]";
}

ScoreSaver::~ScoreSaver(){
    outfile << "]" << std::endl;
    outfile.close();
}