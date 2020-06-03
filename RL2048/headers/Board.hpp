#pragma once

#include <ostream>
#include <array>
#include <tuple>

class Board {

public:
    static constexpr int size = 4;
    static constexpr int zero_tile{1};

private:
    std::array<int, size * size> matrix;

    void slideUp();

    int swipeUp();

    void rotateLeft();

public:
    Board();

    // Indexing

    [[nodiscard]] [[gnu::always_inline]] int &at(int row, int column) {
        return matrix[row * size + column];
    }

    [[nodiscard]] [[gnu::always_inline]] const int &at(int row, int column) const {
        return matrix[row * size + column];
    }

    [[nodiscard]] const int *cbegin() const;

    [[nodiscard]] const int *cend() const;

    // Actions

    [[nodiscard]] std::tuple<Board, int> swipe(int) const;

    [[nodiscard]] std::array<bool, 4> possibleMoves() const;

    void addRandom();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
