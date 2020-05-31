#include <iostream>
#include <algorithm>
#include "../headers/Board.hpp"
#include "../headers/Random.hpp"

const int *Board::cbegin() const {
    return std::cbegin(matrix);
}

const int *Board::cend() const {
    return std::cend(matrix);
}

void Board::rotateLeft() {
    // transpose
    std::swap(matrix[1], matrix[4]);
    std::swap(matrix[2], matrix[8]);
    std::swap(matrix[3], matrix[12]);
    std::swap(matrix[6], matrix[9]);
    std::swap(matrix[7], matrix[13]);
    std::swap(matrix[11], matrix[14]);

    // reverse columns
    for (int col = 0; col < size; col++) {
        std::swap(at(0, col), at(3, col));
        std::swap(at(1, col), at(2, col));
    }
}

void Board::addRandom() {
    for (;;) {
        int pos = random(size * size);
        if (matrix[pos] == 0) {
            matrix[pos] = random(10) ? 2 : 4;
            break;
        }
    }
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
            if (const int val = at(row, col); val != 0) {
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
            if (int &here = at(row, col); here == 0) {
                continue;
            } else if (int &there = at(row + 1, col); here == there) {
                score += (here *= 2);
                there = 0;
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

std::tuple<Board, int> Board::swipe(int direction) const {
    Board b(*this);

    for (int i = 0; i < direction; ++i) {
        b.rotateLeft();
    }

    int score = b.swipeUp();

    if (direction != 0) {
        for (int i = 0; i < 4 - direction; ++i) {
            b.rotateLeft();
        }
    }
    return std::make_tuple(b, score);
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
    for (int i = 0; i < Board::size; ++i) {
        for (int j = 0; j < Board::size; ++j) {
            os << board.at(i, j) << " ";
        }
        os << '\n';
    }
    return os;
}
