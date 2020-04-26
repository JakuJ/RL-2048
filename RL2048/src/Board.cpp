#include <cassert>
#include <iostream>
#include <algorithm>
#include "../headers/Board.hpp"
#include "../headers/random.hpp"

Board::Board() {
    addRandom();
}

Board::Board(const Board &other) {
    std::copy(other.cbegin(), other.cend(), std::begin(matrix));
}

Board &Board::operator=(const Board &other) {
    if (this != &other) {
        std::copy(other.cbegin(), other.cend(), std::begin(matrix));
    }
    return *this;
}

const int &Board::at(int row, int column) const {
#if DEBUG
    assert(row >= 0 && row < size);
    assert(column >= 0 && column < size);
#endif
    return matrix[row * size + column];
}

int &Board::at(int row, int column) {
#if DEBUG
    assert(row >= 0 && row < size);
    assert(column >= 0 && column < size);
#endif
    return matrix[row * size + column];
}

const int *Board::cbegin() const {
    return std::cbegin(matrix);
}

const int *Board::cend() const {
    return std::cend(matrix);
}

void Board::rotateLeft() {
    // transpose
    for (int row = 0; row < size; row++) {
        for (int col = row + 1; col < size; col++) {
            std::swap(at(row, col), at(col, row));
        }
    }
    // reverse columns
    for (int col = 0; col < size; col++) {
        std::swap(at(0, col), at(3, col));
        std::swap(at(1, col), at(2, col));
    }
}

void Board::addRandom() {
    int pos;
    do {
        pos = random(size * size);
    } while (matrix[pos] != 0);

    matrix[pos] = random(10) ? 2 : 4;
}

std::vector<int> Board::possibleMoves() const {
    std::vector<int> possible;
    possible.reserve(4);

    bool can_left = false, can_right = false;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size - 1; col++) {
            int left = at(row, col);
            int right = at(row, col + 1);

            if (left == right && left != 0) {
                can_left = can_right = true;
                goto up_down;
            }

            can_left |= left == 0 && right != 0;
            can_right |= right == 0 && left != 0;
        }
    }

    up_down:

    if (can_left) {
        possible.push_back(3);
    }

    if (can_right) {
        possible.push_back(1);
    }

    bool can_up = false, can_down = false;

    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size - 1; row++) {
            int upper = at(row, col);
            int lower = at(row + 1, col);

            if (upper == lower && upper != 0) {
                can_up = can_down = true;
                goto finish;
            }
            can_up |= upper == 0 && lower != 0;
            can_down |= lower == 0 && upper != 0;
        }
    }

    finish:

    if (can_up) {
        possible.push_back(0);
    }

    if (can_down) {
        possible.push_back(2);
    }

    return possible;
}

void Board::slideUp() {
    for (int col = 0; col < size; col++) {
        int count = 0;

        for (int row = 0; row < size; row++) {
            int val = at(row, col);
            if (val != 0) {
                if (count != row) {
                    at(count, col) = val;
                }
                count++;
            }
        }

        while (count < size) {
            at(count++, col) = 0;
        }
    }
}

int Board::swipeUp() {
    slideUp();
    int score = 0;
    bool merged = false;

    // merging
    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size - 1; row++) {
            if (at(row, col) == 0) {
                continue;
            }
            if (at(row, col) == at(row + 1, col)) {
                score += (at(row, col) *= 2);
                at(row + 1, col) = 0;
                merged = true;
                row++;  // skip the now zero tile
            }
        }
    }

    if (merged) {
        slideUp();
    }

    return score;
}

std::tuple<Board, int> Board::move(int direction) const {
    Board b(*this);

    for (int i = 0; i < direction; i++) {
        b.rotateLeft();
    }

    int score = b.swipeUp();

    if (direction != 0) {
        for (int i = 0; i < 4 - direction; i++) {
            b.rotateLeft();
        }
    }
    return std::make_tuple(b, score);
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (int i = 0; i < Board::size; i++) {
        for (int j = 0; j < Board::size; j++) {
            os << board.at(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}
