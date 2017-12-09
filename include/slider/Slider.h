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

template<typename T, typename S>
class Strategy;

class Slider {
public:
    Slider(SliderPlayer agent, std::size_t size, SliderPlayer player, Strategy<Move, Slider>* strategy);

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
    /// \return a list of possible movements from current state
    std::vector<Move> possible_moves() const;


    /// return a boolean to indicate if this state is already at its final state
    /// \return true if this state has no more children. False otherwise.
    bool is_leaf() const;

    /// return a representation of current state in this slider game
    /// \return Board state
    const Board &get_board() const { return board; }

    /// return a representation of current state in this slider game
    /// \return Board state
    Board &get_board() { return board; }

    /// return the player of this slider state (which player's turn to move in this slider state)
    /// \return player that makes the next turn
    SliderPlayer get_player() const { return player; }

    /// return the agent of this slider game. (Who is behind this side of the board - i.e. the player)
    /// \return player of this slider game
    SliderPlayer get_agent() const { return agent; }

    /// next move this player is going to make (also automatically updates the board state internally)
    /// i.e. the referee doesn't have to ask this slider to update it's internal board with this new move.
    /// \param move this slider player wants to make
    virtual void next_move(Move& move);

    /// returns true if player is ready to move. This is only ever meaningful when Slider is
    /// a non-AI based agent
    /// \return true if this player is ready to make a move, defaults to true
    virtual bool ready_to_move() { return true; }

protected:
    /// size x size board
    std::size_t size;
    Board board;
    // mutable -> @see Slider::is_leaf() (temporarily 'fake' other player to see what moves the have left)
    mutable SliderPlayer player;
    // Which player is this agent representing (this is always a constant)
    const SliderPlayer agent;
    std::vector<SliderMove> moveset{
            SliderMove::Right,
            SliderMove::Up,
            SliderMove::Left,
            SliderMove::Down
    };
    Strategy<Move, Slider>* strategy;
};


#endif //SLIDER_SLIDER_H
