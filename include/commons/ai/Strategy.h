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
    // evaluation function should return a double, and take a const ref to state, with additional param for depth
    typedef double evalf (const State&, size_t);
    virtual std::pair<T, bool> next_move(const State&, evalf) const = 0 ;
};


#endif //SLIDER_STRATEGY_H
