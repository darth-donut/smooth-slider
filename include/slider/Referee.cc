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

    while (!slider_board.has_winner() && !draw_game()) {
        auto pending_move = current_player->next_move();
        // we can be 100% sure it's a legal move because next_move() of slider always checks for illegal moves.
        slider_board.make_move(pending_move);
        // inform other player that we've updated the board
        other_player->update(pending_move);
        swap(current_player, other_player);
    }

    return std::make_pair(slider_board.get_winner(), draw_game());
}

void
Referee::sanity_check() const {
    // check 1 - make sure that the players are enemies of each other
    if (p1->get_agent() == SliderPlayer::Horizontal) {
        assert(p2->get_agent() == SliderPlayer::Vertical);
    } else {
        assert(p2->get_agent() == SliderPlayer::Horizontal);
    }
    // check 2 - make sure that the 1st player is consistent
    assert(p1->get_player() == p2->get_player());
}

std::pair<std::shared_ptr<Slider>, std::shared_ptr<Slider>>
Referee::get_players() const {
    return p1->get_agent() == p1->get_player() ? std::make_pair(p1, p2) : std::make_pair(p2, p1);
}

bool
Referee::draw_game() const {
    return p1->possible_moves().empty() && p2->possible_moves().empty();
}

