#pragma once

#include <ostream>
#include <vector>
#include <tuple>

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

    [[nodiscard]] const int &at(int, int) const;

    [[nodiscard]] int &at(int, int);

    [[nodiscard]] const int *cbegin() const;

    [[nodiscard]] const int *cend() const;

    // Actions

    [[nodiscard]] std::tuple<Board, int> move(int) const;

    [[nodiscard]] std::vector<int> possibleMoves() const;

    void addRandom();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
