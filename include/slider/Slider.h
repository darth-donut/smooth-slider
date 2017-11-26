//
// @author: jiahong
// @date  : 14/11/17 10:00 PM
//

#ifndef SLIDER_SLIDER_H
#define SLIDER_SLIDER_H

#include <vector>
#include <unordered_set>

#include "commons/ai/agent/Minimax.h"
#include "Board.h"
#include "Move.h"
#include "slider_utils.h"


class Slider {
public:
    explicit Slider(std::size_t size, SliderPlayer player);

    /// updates the board according to provided move
    /// \param move Move to make
    /// \return true if the provided move was legal
    bool update(const Move &move);

    /// applies the move to this current Slider board's state (without changing it)
    /// to 'take a peek' at what it would be like after applying this move
    /// \param move Move to make
    /// \return Slider. New Slider state that would be updated if Slider::update(move) was called
    Slider peek_update(const Move &move) const;

    /// with the current board state, returns a vector of possible moves
    /// O(3n) operation. (for each pieces left, for 3 possible moves, check if it is a legal move)
    /// \param n Either Miniax::MAX_NODE or Minimax::Min_NODE - cheaper to find out who's turn is it [default = MAX]
    /// \return a list of possible movements from current state
    std::vector<Move> possible_moves(int n = Minimax<Move, Slider>::MAX_NODE) const;


    /// return a boolean to indicate if this state is already at its final state
    /// \return true if this state has no more children. False otherwise.
    bool is_leaf() const;

    /// return a representation of current state in this slider game
    /// \return Board state
    const Board &get_board() const { return board; }

    /// return a representation of current state in this slider game
    /// \return Board state
    Board &get_board() { return board; }

    /// return the player of this slider state
    /// \return player representative of this slider state
    SliderPlayer get_player() const { return player; }

    /// returns the next move this player is going to make (also automatically updates the board state internally)
    /// i.e. the referee doesn't have to ask this slider to update it's internal board with this new move.
    /// \return Move this slider player wants to maek
    virtual Move next_move();

protected:
    /// size x size board
    std::size_t size;
    Board board;
    // mutable -> @see Slider::is_leaf() (temporarily 'fake' other player to see what moves the have left)
    mutable SliderPlayer player;
    std::vector<SliderMove> moveset{
            SliderMove::Right,
            SliderMove::Left,
            SliderMove::Up,
            SliderMove::Down
    };
private:
    // helper function to get the opposite player
    SliderPlayer other_player(SliderPlayer p) const {
        return p == SliderPlayer::Horizontal ? SliderPlayer::Vertical : SliderPlayer::Horizontal;
    }
};


#endif //SLIDER_SLIDER_H
