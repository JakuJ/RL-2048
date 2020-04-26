#pragma once

#include <ostream>
#include <vector>
#include <tuple>

class Board {

public:
    static constexpr int size = 4;

    void rotateLeft();

private:
    int matrix[size * size] = {0};

    bool slideUp();

    int swipeUp();

public:
    Board();

    Board(const Board &);

    Board(Board &&) = delete;

    Board &operator=(const Board &);

    Board &operator=(Board &&) = delete;

    const int &at(int, int) const;

    int &at(int, int);

    void addRandom();

    std::tuple<Board, int> move(int) const;

    std::vector<int> possibleMoves() const;

    int maximum();

    bool operator==(const Board &rhs) const;

    bool operator!=(const Board &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Board &board);
};
