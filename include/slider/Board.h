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
#include <unordered_set>

#include "Move.h"
#include "slider_utils.h"


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

    /// returns an unordered map of coordinates in the nXn board (Coordinate style) of a given slider player.
    /// This is a O(1) operation. board.make_moves() updates this hash set automatically.
    /// \param player Which player's pieces to return
    /// \return const reference to unordered_set<Move::Coordinate> of player
    const std::unordered_set<Move::Coordinate>& get_piece_positions(SliderPlayer player) const {
        if (player == SliderPlayer::Horizontal)
            return hori_piece_positions;
        return vert_piece_positions;
    }

    std::vector<SliderPiece> &operator[](size_type n) { return board[n]; }

    const std::vector<SliderPiece> &operator[](size_type n) const { return board[n]; }


private:
    std::vector<std::vector<SliderPiece>> board;
    std::size_t size;
    /// horizontal player's pieces in
    /// O(n) access instead of O(n^2) iterating the board each time
    std::unordered_set<Move::Coordinate> hori_piece_positions;
    /// vertical player's pieces in
    /// O(n) access instead of O(n^2) iterating the board each time
    std::unordered_set<Move::Coordinate> vert_piece_positions;


private:
    /// Resets the board to the starting config
    void reset_board();
    /// initialize the set of player positions
    void initialize_piece_positions();

    /// Tells if the move requested by player is to slide player's piece over the board (i.e. the piece got to the goal)
    /// \return True if the player requested a 'goal' move false otherwise
    bool is_edge_move(const Move &move) const {
        return move.get_player() == SliderPlayer::Vertical
               ? move.apply_move().first == -1
               : move.apply_move().second == size;
    }

    /// updates the hash set according to move made (assues that this move is legal) it's up to the caller
    /// to make sure that the move is legal!
    /// \param move A legal move that will update internal hash set
    void update_piece_positions(const Move& move);
};


#endif //SLIDER_BOARD_H
