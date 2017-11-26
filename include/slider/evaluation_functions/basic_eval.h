//
// @author: jiahong
// @date  : 26/11/17 6:44 PM
//

#ifndef SLIDER_BASIC_EVAL_H
#define SLIDER_BASIC_EVAL_H

#include "Slider.h"

double
count_eval(const Slider &state) {
    auto player = state.get_player();
    auto pieces_left = state.get_board().get_piece_positions(player).size();
    // the LESS pieces left, the better.
    return state.get_board().get_size() - pieces_left;
}

#endif //SLIDER_BASIC_EVAL_H
