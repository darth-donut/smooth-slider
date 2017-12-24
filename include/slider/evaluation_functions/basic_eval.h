//
// @author: jiahong
// @date  : 26/11/17 6:44 PM
//

#ifndef SLIDER_BASIC_EVAL_H
#define SLIDER_BASIC_EVAL_H

#include "Slider.h"

double evaluate(const Slider &state, size_t depth);

double pieces_left(const Slider &state, size_t depth);

double enemy_move_count(const Slider &state, size_t depth);

double player_move_count(const Slider &state, size_t depth);

double manhattan_dist(const Slider &state, size_t depth);

double enemy_blocked_pieces(const Slider &state, size_t depth);

double unblockable_path(const Slider &state, size_t depth);

double straight_path_block(const Slider &state, size_t depth);

inline double transform_score(double score, double depth);

double
transform_score(double score, double depth) {
    return score;
}

#endif      // SLIDER_BASIC_EVAL_H
