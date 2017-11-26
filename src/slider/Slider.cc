//
// @author: jiahong
// @date  : 14/11/17 10:00 PM
//

#include <algorithm>
#include <vector>

#include "Slider.h"


Slider::Slider(std::size_t size, SliderPlayer player)
        : size(size),
          board(size),
          player(player) {}

bool
Slider::update(const Move &move) {
    return board.make_move(move);
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
    player = (player == SliderPlayer::Vertical) ? SliderPlayer::Horizontal : SliderPlayer::Vertical;
    // see if opponent has no more moves left
    auto opponent_no_moves = possible_moves().empty();
    player = (player == SliderPlayer::Vertical) ? SliderPlayer::Horizontal : SliderPlayer::Vertical;
    // resume back to original player

    // if BOTH has no moves left, return true => terminal node
    return possible_moves().empty() && opponent_no_moves;


}

Move Slider::next_move() const {
    // TODO: use strategy!
    return {};
}

