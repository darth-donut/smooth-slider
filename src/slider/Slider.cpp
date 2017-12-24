//
// @author: jiahong
// @date  : 14/11/17 10:00 PM
//

#include <algorithm>
#include <cassert>
#include <vector>

#include "Slider.h"
#include "slider/evaluation_functions/basic_eval.h"
#include "util.h"

const std::vector<SliderMove> Slider::moveset = {
        SliderMove::Right,
        SliderMove::Up,
        SliderMove::Left,
        SliderMove::Down
};

Slider::Slider(SliderPlayer agent, const Board& board, SliderPlayer player, Strategy<Move, Slider> *strategy,
               Model *model)
        : agent(agent),
          size(size),
          board(board),
          player(player),
          strategy(strategy),
          model(model) {}

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


std::vector<Move>
Slider::opponent_possible_moves() const {
    auto save_player = player;
    player = other_player(agent);
    // player is now the opponent to this agent, return the possible moves for that state
    auto ret_val = possible_moves();
    player = save_player;
    return ret_val;
}

bool
Slider::is_leaf() const {
    // if either player won - it's a terminal node
    if (board.get_piece_positions(SliderPlayer::Horizontal).empty() ||
        board.get_piece_positions(SliderPlayer::Vertical).empty()) {
        return true;
    }
    // if current round's player has no move left to make, then there's no child node
    return possible_moves().empty();
}

void
Slider::next_move(Move &move) {
    auto ret_val = strategy->next_move(*this, evaluate);
    assert(ret_val.second);
    assert(ret_val.first.get_player() == agent);
    if (!ret_val.second || ret_val.first.get_player() != agent) {
        // should never happen, referee takes care of this for us
        ret_val.first.error("Player has no valid move. Panic!");
    }
    // remember to update our own board!
    update(ret_val.first);
    move = ret_val.first;
}

