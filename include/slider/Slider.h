//
// @author: jiahong
// @date  : 14/11/17 10:00 PM
//

#ifndef SLIDER_SLIDER_H
#define SLIDER_SLIDER_H

#include <vector>
#include "Board.h"
#include "Move.h"



class Slider {
public:
    explicit Slider(std::size_t size);

    /// updates the board according to provided move
    /// \param move Move to make
    /// \return true if the provided move was legal
    bool update(Move move);

    /// with the current board state, returns a vector of possible moves
    /// \return a list of possible movements from current state
    std::vector<Move> possible_moves();

    /// return a representation of current state in this slider game
    /// \return Board state
    const Board &get_board() const { return board; }

    /// return a representation of current state in this slider game
    /// \return Board state
    Board &get_board() { return board; }

private:
    /// size x size board
    std::size_t size;
    Board board;
};


#endif //SLIDER_SLIDER_H
