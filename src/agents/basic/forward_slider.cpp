//
// @author: jiahong
// @date  : 18/12/17 3:07 AM
//

#include <vector>
#include <random>
#include <algorithm>

#include "agents/basic/forward_slider.h"

ForwardSlider::ForwardSlider(SliderPlayer agent, const Board &board, SliderPlayer player)
        : Slider(agent, board, player, nullptr, nullptr) {}

void
ForwardSlider::next_move(Move &move) {
    auto moves = possible_moves();
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(moves.begin(), moves.end(), g);

    for (const auto &m : moves) {
        if (m.get_move() == SliderMove::Up && agent == SliderPlayer::Vertical) {
            update(m);
            move = m;
            return;
        } else if (m.get_move() == SliderMove::Right && agent == SliderPlayer::Horizontal) {
            update(m);
            move = m;
            return;
        }
    }
    if (!moves.empty()) {
        update(moves.front());
        move = moves.front();
    } else {
        Move error_move;
        error_move.error("No possible move for player");
    }
}
