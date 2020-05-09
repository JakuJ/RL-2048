#pragma once

#include <ostream>
#include <vector>
#include <tuple>

class Board {

public:
    static constexpr int size = 4;

	int stage = 0;

private:
    int matrix[size * size] = {0};

    void slideUp();

    int swipeUp();

    void rotateLeft();

public:
    Board();

    Board(const Board &);

    Board(Board &&) = delete;

    Board &operator=(const Board &);

    Board &operator=(Board &&) = delete;

    // Indexing

    [[nodiscard]] inline __attribute__((always_inline)) int &at(int row, int column) {
        return matrix[row * size + column];
    }

    [[nodiscard]] inline __attribute__((always_inline)) const int &at(int row, int column) const {
        return matrix[row * size + column];
    }

    [[nodiscard]] const int *cbegin() const;

    [[nodiscard]] const int *cend() const;

    // Actions

    [[nodiscard]] std::tuple<Board, int> move(int) const;

    [[nodiscard]] std::vector<int> possibleMoves() const;

    void addRandom();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
