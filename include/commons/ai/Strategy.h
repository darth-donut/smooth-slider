//
// @author: jiahong
// @date  : 14/11/17 9:59 PM
//

#ifndef SLIDER_STRATEGY_H
#define SLIDER_STRATEGY_H

#include "Slider.h"

template <typename T, typename State>
class Strategy {
public:
    /// considers the next best move to make
    /// \return the next best move
    virtual T next_move(const State&) const = 0 ;
};


#endif //SLIDER_STRATEGY_H
