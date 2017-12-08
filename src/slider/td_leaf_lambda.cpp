//
// @author: jiahong
// @date  : 9/12/17 4:49 AM
//

#include "td_leaf_lambda.h"

double
sech

TDLeafLambda::TDLeafLambda(Model& model,
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
    constexpr double a  = 1, b = 1;

    // for each weight:
    for (int k = 0; k < model.size(); ++k) {
        // delta w = alpha * sigma{i = 1, N - 1}(a * b * sech^2( b * meta.second) * phi_i(meta.first) * sigma{j = i, N - 1}(lambda^(j-i) * d_j ) )
        // where d_j = move_history[j+1].second - meta.second (i.e. difference between immediate state and current)
        // SIG(t = 1, N - 1): FOR ALL STATES
        double delta_weight = 0;
        for (int i = 0; i < move_history.size() - 1; ++i) {
            // meta.first = state, meta.second = eval value
            auto meta = move_history[i].get_metadata();
            // difference in score between state i+1 and i multiplied by lambda
            double lambda_d_j = 0;
            // SIG(i = t, N - 1): FOR ALL STATES FROM INDEX t ONWARDS
            for (int j = i; j < move_history.size(); ++j) {
                lambda_d_j += pow(lambda, j - i) * (move_history[j+1].get_metadata().second - meta.second);
            }
            // todo: sech2 and phi
            delta_weight += alpha * a * b * sech2(b * meta.second) * phi[i](meta.first) * lambda_d_j;
        }
        model[k] += delta_weight;
    }
}
