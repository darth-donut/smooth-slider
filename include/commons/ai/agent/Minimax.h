//
// @author: jiahong
// @date  : 15/11/17 12:14 AM
//

#ifndef SLIDER_MINIMAX_H
#define SLIDER_MINIMAX_H

#include "Strategy.h"
#include "Move.h"

template <typename T, typename State>
class Minimax : public Strategy<T, State> {
public:
//    explicit Minimax(EvaluationFunction ef);
    T next_move(const State&) const override;
};


template <typename T, typename State>
Move
Minimax<T, State>::next_move(const State&) const {
    // TODO - minimax + pruning
    // make use of template evaluation function
}

#endif //SLIDER_MINIMAX_H
