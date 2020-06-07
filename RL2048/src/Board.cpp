#include <iostream>
#include <algorithm>
#include "../headers/Board.hpp"

template<unsigned int max>
static unsigned int fast_rand() {
    static_assert((max & (max - 1)) == 0, "MAX must be a power of two");
    thread_local static unsigned int seed = 1337;
    seed = 214013 * seed + 2531011;
    return (seed >> 16u) & 0x7FFF & (max - 1);
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
        unsigned int pos = fast_rand<size * size>();
        if (matrix[pos] == zero_tile) {
            matrix[pos] = 2 + 2 * (fast_rand<1024>() < 103);  // approx. 10% chance to get a 4-tile
            break;
        }
    }
}

// An array of 4 booleans is represented as an 32-bit integer starting at optimization level 2
// https://godbolt.org/z/CvbUad
std::array<bool, 4> Board::possibleMoves() const {
    std::array<bool, 4> possible{};
    auto&[can_up, can_right, can_down, can_left] = possible;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size - 1; col++) {
            const int left = at(row, col);
            const int right = at(row, col + 1);

            if (left == right && left != zero_tile) {
                can_left = can_right = true;
                goto up_down;
            }

            can_left |= left == zero_tile && right != zero_tile;
            can_right |= right == zero_tile && left != zero_tile;
        }
    }

    up_down:

    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size - 1; row++) {
            const int upper = at(row, col);
            const int lower = at(row + 1, col);

            if (upper == lower && upper != zero_tile) {
                can_up = can_down = true;
                goto finish;
            }
            can_up |= upper == zero_tile && lower != zero_tile;
            can_down |= lower == zero_tile && upper != zero_tile;
        }
    }

    finish:
    return possible;
}

void Board::slideUp() {
    for (int col = 0; col < size; col++) {
        int count = 0;

        for (int row = 0; row < size; row++) {
            if (const int val = at(row, col); val != zero_tile) {
                if (count != row) {
                    at(count, col) = val;
                }
                count++;
            }
        }

        while (count < size) {
            at(count++, col) = zero_tile;
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
            if (int &here = at(row, col); here == zero_tile) {
                continue;
            } else if (int &there = at(row + 1, col); here == there) {
                score += (here *= 2);
                there = zero_tile;
                merged = true;
                row++;  // skip the now zero tile
            
                // multi-stage
                switch (stage)
				{
				case 0:
					if (merged_value == 1024)
						stage++;
					break;
				case 1:
					if (merged_value == 2048)
						stage++;
					break;
				case 2:
					if (merged_value == 1024)
						stage++;
					break;
				case 3:
					if (merged_value == 4096)
						stage++;
					break;
				default:
					break;
				}
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
