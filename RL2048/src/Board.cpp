#include <cassert>
#include <iostream>
#include <algorithm>
#include "../headers/Board.hpp"
#include "../headers/random.hpp"

Board::Board() {
    addRandom();
}

Board::Board(const Board &other) {
    std::copy(other.matrix, std::end(other.matrix), matrix);
}

Board &Board::operator=(const Board &other) {
    if (this != &other) {
        std::copy(other.matrix, std::end(other.matrix), matrix);
    }
    return *this;
}

const int &Board::at(int row, int column) const {
    assert(row >= 0 && row < size);
    assert(column >= 0 && column < size);
    return matrix[row * size + column];
}

int &Board::at(int row, int column) {
    assert(row >= 0 && row < size);
    assert(column >= 0 && column < size);
    return matrix[row * size + column];
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
    int row, col;
    do {
        row = random(0, 3);
        col = random(0, 3);
    } while (at(row, col) != 0);
    at(row, col) = (random(1, 10) <= 9) ? 2 : 4;
}

std::vector<int> Board::possibleMoves() const {
    std::vector<int> possible;

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

bool Board::slideUp() {
    bool moved = false;

    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size - 1; row++) {
            for (int to_row = row; to_row >= 0; to_row--) {
                if (at(to_row, col) == 0 && at(to_row + 1, col) != 0) {
                    std::swap(at(to_row, col), at(to_row + 1, col));
                    moved = true;
                }
            }
        }
    }

    return moved;
}

int Board::swipeUp() {
    bool moved = slideUp();
    int score = 0;
    bool merged = false;

    // merging
    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size - 1; row++) {
            if (at(row, col) == 0) {
                continue;
            }
            if (at(row, col) == at(row + 1, col)) {
                at(row, col) *= 2;
                at(row + 1, col) = 0;
                score += at(row, col);
                row++;  // skip the now zero tile
                merged = true;
            }
        }
    }

    if (!moved) {
        assert(merged);
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

int Board::maximum() {
    return *std::max_element(matrix, matrix + size * size);
}
