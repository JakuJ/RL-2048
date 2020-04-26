#pragma once

#include <ostream>
#include <vector>
#include <tuple>

class Board {

public:
    static constexpr int size = 4;

private:
    int matrix[size * size] = {0};

    bool slideUp();

    int swipeUp();

    void rotateLeft();

public:
    Board();

    Board(const Board &);

    Board(Board &&) = delete;

    Board &operator=(const Board &);

    Board &operator=(Board &&) = delete;

    [[nodiscard]] const int &at(int, int) const;

    [[nodiscard]] int &at(int, int);

    [[nodiscard]] std::tuple<Board, int> move(int) const;

    [[nodiscard]] std::vector<int> possibleMoves() const;

    void addRandom();

    int maximum();

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
