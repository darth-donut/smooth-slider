//
// @author: jiahong
// @date  : 14/11/17 9:59 PM
//

#ifndef SLIDER_STRATEGY_H
#define SLIDER_STRATEGY_H

#include "Slider.h"
#include "Move.h"

class Strategy {
public:
    /// considers the next best move to make
    /// \return the next best move
    virtual Move next_move(const Slider&) = 0;
};


#endif //SLIDER_STRATEGY_H
