//
// @author: jiahong
// @date  : 9/12/17 4:49 AM
//

#include <vector>

#include "td_leaf_lambda.h"
#include "util.h"


TDLeafLambda::TDLeafLambda(Model &model,
                           std::mutex &model_lock,
                           const std::vector<Move> &move_history)
        : model(model),
          model_lock(model_lock),
          move_history(move_history) {}

void
TDLeafLambda::update_weights() {
    constexpr double alpha = .1;
    constexpr double lambda = .7;
    std::vector<double> lambda_array;
    for (int t = 0; t < move_history.size() - 1; ++t) {
        double t_lambda_sum = 0;
        for (int i = t; i < move_history.size() - 1; ++i) {
            t_lambda_sum += std::pow(lambda, i - t) *
                            (std::get<V_INDEX>(move_history[i + 1].get_metadata()) -
                             std::get<V_INDEX>(move_history[i].get_metadata()));
        }
        lambda_array.push_back(t_lambda_sum);
    }

    // for each weight:
    for (int k = 0; k < model.size(); ++k) {
        // delta w = alpha * sigma{t = 1, N - 1}(a * b * sech^2( b * meta.evalfscore) * phi_k(*meta.first, meta.depth) * sigma{i = t, N - 1}(lambda^(i-t) * d_i ) )
        // SIG(t = 1, N - 1): FOR ALL STATES
        double delta_weight = 0;
        for (int t = 0; t < move_history.size() - 1; ++t) {
            // meta.first = state, meta.second = eval value
            auto meta = move_history[t].get_metadata();
            delta_weight +=
                    alpha * model.a * model.b * sech2(model.b * std::get<V_INDEX>(meta)) *
                    model.phi[k](*std::get<STATE_INDEX>(meta), std::get<DEPTH_INDEX>(meta)) *
                    lambda_array[t];
        }
        model[k] += delta_weight;
    }
    std::lock_guard<std::mutex> guard_flush(model_lock);
    model.flush();
}
