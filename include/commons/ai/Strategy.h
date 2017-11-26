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
    virtual std::pair<T, bool> next_move(const State&, double (*eval) (const State&)) const = 0 ;
};


#endif //SLIDER_STRATEGY_H
