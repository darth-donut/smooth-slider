//
// @author: jiahong
// @date  : 14/11/17 10:41 PM
//

#ifndef SLIDER_BOARD_H
#define SLIDER_BOARD_H

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Move.h"

/*
 * (0,0) -----------------> (N,0)
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * v                        v
 * (0, N) ----------------> (N,N)
 */

enum class SliderPiece : char {
    Vertical, Horizontal, Blank, Block
};

class Board {
    friend std::ostream &operator<<(std::ostream &, const Board &);

public:
    typedef std::vector<SliderPiece>::size_type size_type;

    /// Creates a board from a supplied string representation of a string
    /// \param board string representation of a board - the size will be automatically deduced
    explicit Board(const std::string &board);

    /// Creates a board of size - size x size with initial configurations
    /// \param size size of board ( a size x size board)
    explicit Board(size_type);

    /// Makes a specified move. Returns boolean to indicate move status
    /// \param move Move to make
    /// \return true if move was made successfully, false otherwise
    bool make_move(const Move &move);

    /// For a given move, query if it's a legal move.
    /// \param move Move to make
    /// \return true if the move is legal
    bool is_legal(const Move &move) const;

    /// returns the size of this board
    /// \return size of board. The dimension of the board is size x size
    size_type get_size() const { return size; }

    std::vector<SliderPiece> &operator[](size_type n) { return board[n]; }

    const std::vector<SliderPiece> &operator[](size_type n) const { return board[n]; }


private:
    std::vector<std::vector<SliderPiece>> board;
    std::size_t size;

private:
    /// resets the board to the starting config
    void reset_board();

};


#endif //SLIDER_BOARD_H
