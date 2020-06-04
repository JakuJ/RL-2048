#pragma once

#include <ostream>
#include <array>
#include <tuple>

class Board {

public:
    static constexpr unsigned int size = 4;
    static constexpr int zero_tile{1};

private:
    std::array<int, size * size> matrix{};

    void slideUp();

    int swipeUp();

    void rotateLeft();

public:
    Board() {
        matrix.fill(zero_tile);
    }

    // Indexing

    [[nodiscard]] [[gnu::always_inline]] constexpr int &at(int row, int column) {
        return matrix[row * size + column];
    }

    [[nodiscard]] [[gnu::always_inline]] constexpr int const &at(int row, int column) const {
        return matrix[row * size + column];
    }

    [[nodiscard]] int max_tile() const {
        return *std::max_element(matrix.cbegin(), matrix.cend());
    }

    // Actions

    [[nodiscard]] std::tuple<Board, int> swipe(int) const;

    [[nodiscard]] std::array<bool, 4> possibleMoves() const;

    void addRandom();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
