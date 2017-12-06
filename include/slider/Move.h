//
// @author: jiahong
// @date  : 14/11/17 10:11 PM
//

#ifndef SLIDER_MOVE_H
#define SLIDER_MOVE_H

#include <limits>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <cctype>
#include <iostream>

#include "slider_utils.h"
#include "commons/util.h"

#define ALPHA_START 65      // A
#define NUMERIC_START 48    // 0

/// Immutable class Move
class Move {
    friend std::ostream &operator<<(std::ostream &os, const Move &move);

public:
    typedef long int size_type;
    using Coordinate = std::pair<size_type, size_type>;

    Move() = default;


    /// Constructs a move consisting of a player (that was responsible for this move)
    /// and the directional move itself from specified coordinate
    /// \param player Player responsible for this move
    /// \param move Direction of type SliderMove
    /// \param coord Current coordinate
    Move(SliderPlayer player, SliderMove move, Coordinate coord)
            : player(player), move(move), coord(std::move(coord)) {}

    /// Constructs a move consisting of a player (that was responsible for this move)
    /// and the directional move itself from specified coordinate from a string representation
    /// \param user_input String representation of
    /// \param board_size current board size, used to check for bounds
    Move(SliderPlayer player, const std::string &user_input, size_type board_size)
            : player(player) {
        auto tokens = tokenize(user_input, ',');
        if (tokens.size() != 2) {
            throw std::domain_error(
                    "Unrecognized number of arguments, expected <AlphabetNumeric>,<UP|DOWN|LEFT|RIGHT>");
        }

        deduce_coord(tokens[0], board_size);
        deduce_move(tokens[1]);

    }

    Move(SliderPlayer player, Coordinate init, Coordinate final, size_type board_size)
            : player(player), coord(init) {
        if (init.first < 0 || init.first >= board_size || final.first < 0 || final.first > board_size
            || init.second < 0 || init.second >= board_size || final.second < 0 || final.second > board_size) {
            throw std::domain_error("Move not allowed");
        }
        if (std::abs(init.first - final.first) > 1 || std::abs(init.second - final.second) > 1) {
            throw std::domain_error("Can't move more than a step in one turn");
        }
        if ((init.first > final.first) && (init.second == final.second)) {
            move = SliderMove::Up;
        } else if ((init.first < final.first) && (init.second == final.second)) {
            move = SliderMove::Down;
        } else if ((init.second > final.second) && (init.first == final.first)) {
            move = SliderMove::Left;
        } else if ((init.second < final.second) && (init.first == final.first)) {
            move = SliderMove::Right;
        } else {
            throw std::domain_error("Move not allowed");
        }
    }

    /// Gets the player that was responsible for making this move
    /// \return player that was responsible for making this move of type SliderPlayer
    SliderPlayer get_player() const { return player; }

    /// Gets the directional move that some player made
    /// \return Directional move SliderMove
    SliderMove get_move() const { return move; }

    /// Gets the coordinate of this move (starting coordinate)
    /// The final destination of this move is coord + get_move()
    /// \return coordinate of current initial move
    const Coordinate &get_coord() const { return coord; }

    /// Applies the supplied move to current coordinate and returns the new coordinate
    /// It's up to the user to check that the values are in bounds of the board / game. However, it does
    /// check and make sure that the bounds are not violated. The moves will not reach negative coordinates,
    /// nor will it overflow the specified size_type limits. Check this using std::numeric_limits<size_type>::max().
    /// In the event of overflow or negative values, exceptions will be thrown
    /// \return New Coordinate after move is applied to original coordinate. Under/overflow exceptions otherwise
    inline Coordinate apply_move() const;

private:
    SliderPlayer player = SliderPlayer::Horizontal;
    SliderMove move = SliderMove::Right;
    Coordinate coord;

private:
    /// converts string representation of move into SliderMove type
    /// \param input String type of either up, down, left, or right (case-insensitive)
    inline void deduce_move(const std::string &input);

    /// converts string representation of coordinate into Coordinate type
    /// \param input String type of A(65) ... (65 + board_size) followed immediately by column digit (0-index)
    /// \param board_size the current size of slider board
    inline void deduce_coord(const std::string &input, size_type board_size);
};

Move::Coordinate
Move::apply_move() const {
    short x = 0, y = 0;
    switch (move) {
        case SliderMove::Up:
            x = -1;
            break;
        case SliderMove::Down:
            x = 1;
            break;
        case SliderMove::Right:
            y = 1;
            break;
        case SliderMove::Left:
            y = -1;
            break;
    }

    constexpr auto max_lim = std::numeric_limits<size_type>::max();
    if ((coord.first == -1 && x == -1)
        || (coord.second == 0 && y == -1)) {
        throw std::underflow_error("Move is underflowing from minimum allowed unsigned integer value");
    } else if ((coord.first == max_lim && x == 1)
               || (coord.second == max_lim) && y == 1) {
        throw std::overflow_error("Move is overflowing from maximum allowed unsigned integer value");
    }
    return std::make_pair(coord.first + x, coord.second + y);
}

void
Move::deduce_move(const std::string &input) {
    std::string str_move;
    // canonical form = lower casing
    std::transform(input.cbegin(), input.cend(), std::back_inserter(str_move), [](char c) { return std::tolower(c); });

    if (str_move == "down") {
        move = SliderMove::Down;
    } else if (str_move == "up") {
        move = SliderMove::Up;
    } else if (str_move == "left") {
        move = SliderMove::Left;
    } else if (str_move == "right") {
        move = SliderMove::Right;
    } else {
        throw std::domain_error("Unrecognized input " + str_move + ", expected either: (case-insensitive) up, down,"
                " left, or right");
    }
}

void
Move::deduce_coord(const std::string &input, size_type board_size) {
    if (!std::isalpha(input[0])) {
        throw std::domain_error("Unrecognized alphabet " + std::string(1, input[0]));
    }
    if (!std::isdigit(input[1])) {
        throw std::domain_error("Unrecognized numeric " + std::string(1, input[1]));
    }

    auto row = static_cast<size_type>(std::toupper(input[0])) - ALPHA_START;
    auto col = static_cast<size_type>(input[1]) - NUMERIC_START;

    // bounds check
    if (row >= board_size) {
        throw std::range_error("Alphabet " + std::string(1, input[0]) + " is out of range");
    }
    if (col >= board_size) {
        throw std::range_error("Numeric " + std::string(1, input[2]) + " is out of range");
    }

    this->coord = std::make_pair(row, col);
}

namespace std {
    template<>
    struct hash<Move::Coordinate> {
        typedef size_t result_type;
        typedef Move::Coordinate argument_type;

        size_t operator()(const Move::Coordinate &coord) const {
            size_t res = 17;
            res += 31 * hash<Move::size_type>()(coord.first);
            res += 31 * hash<Move::size_type>()(coord.second);
            return res;
        }
    };
}


inline std::ostream
&operator<<(std::ostream &os, const Move &move) {
    std::string p = (move.player == SliderPlayer::Horizontal) ? "Hori" : "Vert";
    std::string m;
    switch (move.move) {
        case SliderMove::Up:
            m = "up";
            break;
        case SliderMove::Down:
            m = "down";
            break;
        case SliderMove::Right:
            m = "right";
            break;
        case SliderMove::Left:
            m = "left";
            break;
    }
    return os << p << " making move: " << m << " from coord: " << move.coord.first
              << ", " << move.coord.second;
}

#endif //SLIDER_MOVE_H
