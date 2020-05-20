#pragma once

#include <ostream>
#include <vector>
#include <tuple>

#define ALWAYS_INLINE __attribute__((always_inline))

class Board {

public:
    static constexpr int size = 4;

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

    [[nodiscard]] inline ALWAYS_INLINE int &at(int row, int column) {
        return matrix[row * size + column];
    }

    [[nodiscard]] inline ALWAYS_INLINE const int &at(int row, int column) const {
        return matrix[row * size + column];
    }

    [[nodiscard]] const int *cbegin() const;

    [[nodiscard]] const int *cend() const;

    // Actions

    [[nodiscard]] std::tuple<Board, int> swipe(int) const;

    [[nodiscard]] std::vector<int> possibleMoves() const;

    void addRandom();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
