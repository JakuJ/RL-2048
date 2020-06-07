#!/bin/bash
set -e
(
  mkdir -p cmake-build-release
  cd cmake-build-release

  cmake -DCMAKE_CXX_COMPILER=clang++ ..
  make -j4

  mkdir -p "../tuples"
  zsh -c "time ./RL2048 $1 $2 ../log.csv ../tuples/tuple"
)
