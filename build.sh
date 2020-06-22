#!/bin/bash
set -e

mkdir -p build_dir

(
  cd build_dir

  cmake -DCMAKE_CXX_COMPILER=clang++ ..
  make -j

  mkdir -p "../tuples"
  ./RL2048 "$1" "$2" ../log.csv ../tuples/tuple
)
