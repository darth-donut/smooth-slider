//
// @author: jiahong
// @date  : 9/12/17 4:49 AM
//

#ifndef SLIDER_TD_LEAF_LAMBDA_H
#define SLIDER_TD_LEAF_LAMBDA_H

#include <mutex>
#include <vector>

#include "model.h"
#include "Move.h"

class TDLeafLambda {
public:
    TDLeafLambda(Model& model, std::mutex& model_lock, const std::vector<Move>& move_history);
    void update_weights();

private:
    const std::vector<Move>& move_history;
    Model& model;
    // so that even in multi-threaded environments, we will not override the output file with race conditions
    std::mutex& model_lock;
};


#endif //SLIDER_TD_LEAF_LAMBDA_H
