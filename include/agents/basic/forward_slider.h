//
// @author: jiahong
// @date  : 18/12/17 3:07 AM
//

#ifndef SLIDER_FORWARD_SLIDER_H
#define SLIDER_FORWARD_SLIDER_H


#include "slider/Slider.h"

/// Forward slider always aim to make the first random move that advances a random piece towards the goal
/// Goal = forward for vertical player and right for horizontal player
class ForwardSlider : public Slider {
public:
    ForwardSlider(SliderPlayer agent, const Board &board, SliderPlayer player);
    void next_move(Move &move) override;

};


#endif //SLIDER_FORWARD_SLIDER_H
