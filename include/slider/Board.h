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
    /// \param blocks true if the board should randomize block positions
    explicit Board(size_type size, bool blocks = true);

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
    size_type size() const { return bsize; }

    /// returns an unordered set of coordinates in the nXn board (Coordinate format) of a given slider player.
    /// This is a O(1) operation. board.make_moves() updates this hash set automatically.
    /// \param player Which player's pieces to return
    /// \return const reference to unordered_set<Move::Coordinate> of player
    const std::unordered_set<Move::Coordinate> &get_piece_positions(SliderPlayer player) const {
        return player == SliderPlayer::Horizontal
               ? hori_piece_positions
               : vert_piece_positions;
    }

    /// returns the winner - undefined behaviour if there's no winner. Always check with Board::has_winner before
    /// calling this method
    /// \return
    SliderPlayer get_winner() const {
        return hori_piece_positions.empty() ? SliderPlayer::Horizontal : SliderPlayer::Vertical;
    }

    /// returns true if there's a winner. I.e. either all horizontal pieces or vertical pieces have gone over the
    /// board.
    /// \return  True if there's a winner - false otherwise
    bool has_winner() const { return hori_piece_positions.empty() || vert_piece_positions.empty(); }

    /// Tells if the move requested by player is to slide player's piece over the board (i.e. the piece got to the goal)
    /// \param move assumes that this move is a legal move (i.e. is_legal(move)) is called beore is_edge_move is called
    /// \return True if the player requested a 'goal' move false otherwise
    bool is_edge_move(const Move &move) const {
        return move.get_player() == SliderPlayer::Vertical
               ? move.apply_move().first == -1
               : move.apply_move().second == bsize;
    }

    std::vector<SliderPiece> &operator[](size_type n) { return board[n]; }

    const std::vector<SliderPiece> &operator[](size_type n) const { return board[n]; }


private:
    std::vector<std::vector<SliderPiece>> board;
    std::size_t bsize;
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


    /// updates the hash set according to move made (assues that this move is legal) it's up to the caller
    /// to make sure that the move is legal!
    /// \param move A legal move that will update internal hash set
    void update_piece_positions(const Move &move);

    /// creates random blocks in the board
    void randomize_blocks();
};


#endif //SLIDER_BOARD_H
