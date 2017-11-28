//
// @author: jiahong
// @date  : 14/11/17 10:00 PM
//

#include <algorithm>
#include <cassert>
#include <vector>

#include "Slider.h"
#include "commons/ai/agent/Minimax.h"
#include "slider/evaluation_functions/basic_eval.h"
#include "util.h"


Slider::Slider(SliderPlayer agent, std::size_t size, SliderPlayer player, Strategy<Move, Slider> *strategy)
        : agent(agent),
          size(size),
          board(size),
          player(player),
          strategy(strategy) {}

bool
Slider::update(const Move &move) {
    auto ret = board.make_move(move);
    // the board has been updated => the next player to move is flipped
    if (ret) {
        player = other_player(player);
    }
    return ret;
}

Slider
Slider::peek_update(const Move &move) const {
    Slider copy(*this);
    copy.update(move);
    return copy;
}

std::vector<Move>
Slider::possible_moves() const {
    std::vector<Move> potential_moves;
    for (const auto &coord : board.get_piece_positions(player)) {
        for (auto move : moveset) {
            if ((player == SliderPlayer::Vertical && move == SliderMove::Down) ||
                (player == SliderPlayer::Horizontal && move == SliderMove::Left)) {
                // these move-sets aren't allowed!
                continue;
            }
            Move possible_move(player, move, coord);
            if (board.is_legal(possible_move)) {
                potential_moves.push_back(std::move(possible_move));
            }
        }
    }
    return potential_moves;
}

bool
Slider::is_leaf() const {
    // if either player won - it's a terminal node
    if (board.get_piece_positions(SliderPlayer::Horizontal).empty() ||
        board.get_piece_positions(SliderPlayer::Vertical).empty()) {
        return true;
    }

    // temporarily 'fake' other player
    player = other_player(player);
    // see if opponent has no more moves left
    auto opponent_no_moves = possible_moves().empty();
    player = other_player(player);
    // resume back to original player

    // if BOTH has no moves left, return true => terminal node
    return possible_moves().empty() && opponent_no_moves;


}

Move
Slider::next_move() {
    if (strategy) {
        auto ret_val = strategy->next_move(*this, compound_eval);
        assert(ret_val.second);
        // remember to update our own board!
        update(ret_val.first);
        return ret_val.first;
    } else {
        return {};
    }
}

