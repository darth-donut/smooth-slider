//
// @author: jiahong
// @date  : 9/12/17 4:49 AM
//

#include <vector>

#include "td_leaf_lambda.h"
#include "basic_eval.h"
#include "util.h"


TDLeafLambda::TDLeafLambda(Model &model,
                           const std::vector<Move> &move_history)
        : model(model),
          move_history(move_history) {}

const Model &
TDLeafLambda::update_weights() {
    using history_t = std::vector<Move>::size_type;
    using model_t = Model::size_type;
    constexpr double alpha = .1;
    constexpr double lambda = .7;
    std::vector<double> lambda_array;
    std::vector<double> raw_eval_score;

    // compute lambda array
    for (history_t t = 0; t < (move_history.size() - 1); ++t) {
        double t_lambda_sum = 0;
        for (history_t i = t; i < (move_history.size() - 1); ++i) {
            double temporal_difference = std::get<V_INDEX>(move_history[i + 1].get_metadata())
                                         - std::get<V_INDEX>(move_history[i].get_metadata());
            t_lambda_sum += std::pow(lambda, i - t) * temporal_difference;
        }
        lambda_array.push_back(t_lambda_sum);
    }
    assert(lambda_array.size() == (move_history.size() - 1));


    // compute raw_eval_score array
    for (history_t t = 0; t < (move_history.size() - 1); ++t) {
        double raw_score = 0;
        const auto &meta = move_history[t].get_metadata();
        const auto &state_ptr = std::get<STATE_INDEX>(meta);
        const auto depth = std::get<DEPTH_INDEX>(meta);
        for (model_t k = 0; k < model.size(); ++k) {
            raw_score += (model.phi[k](*state_ptr, depth) * model[k]);
        }
        raw_eval_score.push_back(transform_score(raw_score, depth));
    }

    // tdl loops from t=1..N-1 (skipping the last eval function) because we've hard coded the WIN_STATE_SCORE and
    // LOSE_STATE_SCORE (basic_eval.cpp). TDL's aim is to adjust the weights so that they approach these scores
    assert(std::get<STATE_INDEX>(move_history.back().get_metadata())->has_winner());
    assert(raw_eval_score.size() == (move_history.size() - 1));
//
//    std::cout << "---------------------------------------------------\n";
//    std::cout << "Model size: " << model.size() << '\n' << "move_history.size(): " << move_history.size() << std::endl;
//    std::cout << "---------------------------------------------------\n";
    for (model_t k = 0; k < model.size(); ++k) {
        double delta_weight = 0;
        for (history_t t = 0; t < (move_history.size() - 1); ++t) {
            // meta.first = state, meta.second = eval value
            const auto &meta = move_history[t].get_metadata();
            const auto &state_ptr = std::get<STATE_INDEX>(meta);
            const auto &eval_score = std::get<V_INDEX>(meta);
            const auto depth = std::get<DEPTH_INDEX>(meta);
//            std::cout << sech2(model.b * raw_eval_score[t]) << std::endl;
            delta_weight += (model.a * model.b * model.phi[k](*state_ptr, depth) * sech2(model.b * raw_eval_score[t]) *
                             lambda_array[t]);
        }
        model[k] += (alpha * delta_weight);
    }
    return model;
}
