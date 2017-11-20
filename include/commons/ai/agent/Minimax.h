//
// @author: jiahong
// @date  : 15/11/17 12:14 AM
//

#ifndef SLIDER_MINIMAX_H
#define SLIDER_MINIMAX_H

#include "commons/ai/efunc/EvaluationFunction.h"
#include "Strategy.h"
#include "Move.h"

template <typename T, typename State, typename K>
class Minimax : public Strategy<T, State> {
public:
    explicit Minimax(EvaluationFunction<State, K>  &ef) : ef(ef) {}
    T next_move(const State&) const override;
private:
    const EvaluationFunction &ef;
};


template <typename T, typename State, typename K>
Move
Minimax<T, State, K>::next_move(const State& state) const {
    // TODO - minimax + pruning
    // make use of template evaluation function

}

#endif //SLIDER_MINIMAX_H
