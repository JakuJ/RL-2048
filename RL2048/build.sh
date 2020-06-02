#!/bin/bash
set -e
(
  mkdir -p cmake-build-debug
  cd cmake-build-debug
  if [ "$(uname -s)" == 'Darwin' ]; then
    cmake -DCMAKE_CXX_COMPILER=clang++ ..
  else
    cmake ..
  fi
  make

  mkdir -p "../tuples"
  zsh -c "time ./RL2048 $1 $2 ../log.csv ../tuples/tuple"
)
