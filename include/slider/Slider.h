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

class Model;

class Slider {
public:
    static const std::vector<SliderMove> moveset;
public:
    Slider(SliderPlayer agent, const Board &board, SliderPlayer player, Strategy<Move, Slider> *strategy,
           Model *model);

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

    /// with the current board state, returns a vector of possible moves for the enemy
    /// O(3n) operation. (for each pieces left, for 3 possible moves, check if it is a legal move)
    /// \return a list of possible movements from current state
    std::vector<Move> opponent_possible_moves() const;


    /// return a boolean to indicate if this state is already at its final state
    /// \return true if this state has no more children. False otherwise.
    bool is_leaf() const;

    /// return a boolean to indicate if this state is in a draw state.
    /// \return true if this state has both players with empty legal moves left
    bool is_draw() const;

    /// returns a boolean to indicate if there's a winner in this state
    /// \return true if this state has a winner
    bool has_winner() const;

    /// returns the winner of this match. return value is unreliable if there's no winner. Always check with
    /// Slider::has_winner before calling this method.
    /// \return winner of this game. Undefined result if there's no winner.
    SliderPlayer get_winner() const;

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

    /// returns a const pointer(low-level const) to the current model that this slider player is using.
    /// This is typically used when the evaluation function wants to access this slider's
    /// model (which in turn consists of the weights and state evaluation functions)
    /// \return const pointer(low-level const) to model that is attached to this slider player.
    Model *const get_model() const { return model; }

    /// returns the hand to this player because the opponent has no valid moves left to make
    /// side effect: returns hand to this player internally
    void return_round() { player = agent; }
    /// returns the hand to this player because the opponent has no valid moves left to make

    /// opposite of return_round. This player loses this round
    void lose_round() { player = other_player(agent); }

    /// next move this player is going to make (also automatically updates the board state internally)
    /// i.e. the referee doesn't have to ask this slider to update it's internal board with this new move.
    /// \param move this slider player wants to make
    virtual void next_move(Move &move);

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
    Strategy<Move, Slider> *strategy;
    Model *model;
};


inline bool
Slider::is_leaf() const {
    // if either player won - it's a terminal node
    if (board.get_piece_positions(SliderPlayer::Horizontal).empty() ||
        board.get_piece_positions(SliderPlayer::Vertical).empty()) {
        return true;
    }
    // if current round's player has no move left to make, then there's no child node
    return possible_moves().empty();
}

inline bool
Slider::is_draw() const {
    return possible_moves().empty() && opponent_possible_moves().empty();
}

inline bool
Slider::has_winner() const {
    return board.has_winner();
}

inline SliderPlayer
Slider::get_winner() const {
    return board.get_winner();
}

#endif //SLIDER_SLIDER_H
