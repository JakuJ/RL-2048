#!/bin/bash
set -e
(
    cd cmake-build-debug
    cmake -DCMAKE_CXX_COMPILER=g++-9 ..
    make
    zsh -c "time ./RL2048 $1 $2 ../log.csv"
    cd ..
    python3 plot.py log.csv 
)
