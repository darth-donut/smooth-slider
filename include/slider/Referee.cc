//
// @author: jiahong
// @date  : 26/11/17 6:57 PM
//

#include <cassert>

#include "Referee.h"

std::pair<SliderPlayer, bool>
Referee::start_game() {
    using std::swap;
    // current_player is the player that has to make the move this round
    // other_player is the 'other" player that made its move in the previous round
    auto tmp = get_players();
    auto current_player = tmp.first;
    auto other_player = tmp.second;

    while (!slider_board.has_winner() || !draw_game()) {
        auto pending_move = current_player->next_move();
        if (slider_board.is_legal(pending_move)) {
            slider_board.make_move(pending_move);
            other_player->update(pending_move);
        } else {
            // TODO: handle incorrect move from player
        }
        swap(current_player, other_player);
    }

    return std::make_pair(slider_board.get_winner(), draw_game());
}

void
Referee::sanity_check() const {
    // check 1 - make sure that the players are enemies of each other
    if (p1->get_player() == SliderPlayer::Horizontal) {
        assert(p2->get_player() == SliderPlayer::Vertical);
    } else {
        assert(p2->get_player() == SliderPlayer::Horizontal);
    }
}

std::pair<std::shared_ptr<Slider>, std::shared_ptr<Slider>>
Referee::get_players() const {
    p1->get_player() == SliderPlayer::Horizontal ? std::make_pair(p1, p2) : std::make_pair(p2, p1);
}

bool
Referee::draw_game() const {
    return p1->possible_moves().empty() && p2->possible_moves().empty();
}

