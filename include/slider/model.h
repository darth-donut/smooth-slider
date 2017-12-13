//
// @author: jiahong
// @date  : 9/12/17 4:53 AM
//

#ifndef SLIDER_MODEL_H
#define SLIDER_MODEL_H

#include <vector>
#include <string>
#include <unordered_map>

#include "slider/evaluation_functions/basic_eval.h"

class Slider;

class Model {
public:
    typedef double evalf(const Slider &, std::size_t);

    typedef std::unordered_map<std::string, evalf *> EvalDict;
    typedef std::vector<double>::size_type size_type;
public:
    Model() : phi({pieces_left}), weights({1}), a(1), b(1) {}

    Model(const std::string &model_file);

    double &operator[](size_type i) { return weights[i]; };

    size_type size() const { return weights.size(); }

    void flush() const;

public:
    std::vector<evalf *> phi;
    double a;
    double b;

private:
    std::vector<double> weights;
    // phi function in string format
    std::vector<std::string> phis;
    const std::string fname;
    EvalDict eval_dict = {
            {"pieces_left",          pieces_left},
            {"enemy_move_count",     enemy_move_count},
            {"player_move_count",    player_move_count},
            {"manhattan_dist",       manhattan_dist},
            {"enemy_blocked_pieces", enemy_blocked_pieces},
            {"unblockable_path",     unblockable_path},
            {"straight_path_block",     straight_path_block}
    };
};


#endif //SLIDER_MODEL_H
