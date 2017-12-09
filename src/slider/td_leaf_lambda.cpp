//
// @author: jiahong
// @date  : 9/12/17 4:49 AM
//

#include <vector>

#include "td_leaf_lambda.h"


TDLeafLambda::TDLeafLambda(Model &model,
                           std::mutex &model_lock,
                           const std::vector<Move> &move_history)
        : model(model),
          model_lock(model_lock),
          move_history(move_history) {}

void
TDLeafLambda::update_weights() {
    constexpr double alpha = 1.0;
    constexpr double lambda = .7;
    // todo: tune this to suitable values for a and b
    constexpr double a = 1, b = 1;
    std::vector<double> lambda_array;

    for (int t = 0; t < move_history.size() - 1; ++t) {
        double t_lambda_sum = 0;
        for (int i = t; i < move_history.size(); ++i) {
            t_lambda_sum += pow(lambda, i - t) *
                               (move_history[i + 1].get_metadata().second - move_history[i].get_metadata().second);
        }
        lambda_array.push_back(t_lambda_sum);
    }

    // for each weight:
    for (int k = 0; k < model.size(); ++k) {
        // delta w = alpha * sigma{i = 1, N - 1}(a * b * sech^2( b * meta.second) * phi_i(meta.first) * sigma{j = i, N - 1}(lambda^(j-i) * d_j ) )
        // where d_j = move_history[j+1].second - meta.second (i.e. difference between immediate state and current)
        // SIG(t = 1, N - 1): FOR ALL STATES
        double delta_weight = 0;
        for (int t = 0; t < move_history.size() - 1; ++t) {
            // meta.first = state, meta.second = eval value
            auto meta = move_history[t].get_metadata();
            // todo: sech2 and phi
            delta_weight += alpha * a * b * sech2(b * meta.second) * phi[k](*meta.first) * lambda_array[t];
        }
        model[k] += delta_weight;
    }
}
