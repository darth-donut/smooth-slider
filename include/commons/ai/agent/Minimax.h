//
// @author: jiahong
// @date  : 15/11/17 12:14 AM
//

#ifndef SLIDER_MINIMAX_H
#define SLIDER_MINIMAX_H

#include <vector>
#include <limits>

#include "Strategy.h"
#include "Move.h"

/// Constructs a minimax tree with alpha-beta pruning.
/// Type constraints on Move requires a default constructor and a method
/// named add_metadata(std::pair<std::shared_ptr<State>, double>)
/// Type constraints on State requires state::possible_moves(), state::peek_update(const T&), state::is_leaf()
/// \tparam Move Type move. Returns this move as the best move to make given a current state. Move should have:
/// <br>
/// - add_metadata(std::pair<std::shared_ptr<State>, double>) that adds a shared_ptr to the principal variation state
///   and also the leaf evaluation value of type double (This is useful for TDLeaf(lambda) parameters.
/// \tparam State Type State. State requires the following to be defined
/// <br>
/// - possible_moves() - returns a vector of moves (of type mMove) that can be made legally from a given state
/// <br>
/// - peek_update() - return a new state after applying the provided move.
/// <br>
/// - is_leaf() - returns true if this state is a leaf state, false otherwise
template<typename Move, typename State>
class Minimax : public Strategy<Move, State> {
public:
    typedef size_t size_type;
public:
    Minimax() = default;

    explicit Minimax(size_type max_depth) : max_depth(max_depth) {}

    std::pair<Move, bool> next_move(const State &, typename Strategy<Move, State>::evalf) const override;

private:
    static constexpr double INF = std::numeric_limits<double>::max();
    static constexpr double NINF = std::numeric_limits<double>::min();
    size_type max_depth = std::numeric_limits<size_type>::max();
private:
    std::pair<std::shared_ptr<State>, double> maxValue(const State &state, double alpha, double beta, size_type depth,
                                                       typename Strategy<Move, State>::evalf) const;

    std::pair<std::shared_ptr<State>, double> minValue(const State &state, double alpha, double beta, size_type depth,
                                                       typename Strategy<Move, State>::evalf) const;
};

template<typename T, typename State>
std::pair<T, bool>
Minimax<T, State>::next_move(const State &state, typename Strategy<T, State>::evalf eval) const {
    double v = NINF;
    double alpha = NINF;
    double beta = INF;
    auto possible_moves = state.possible_moves();
    T best_move;
    if (possible_moves.size() == 0) {
        return std::make_pair(best_move, false);
    }
    std::shared_ptr<State> principal_var = nullptr;
    for (const auto &move : possible_moves) {
        auto ret_val = minValue(state.peek_update(move), alpha, beta, 1, eval);
        double v_prime = ret_val.second;
        if (v_prime > v) {
            principal_var = ret_val.first;
            v = v_prime;
            best_move = move;
        }
        alpha = std::max(v, alpha);
    }
    best_move.add_metadata(std::make_pair(principal_var, v));
    return std::make_pair(best_move, true);
}

template<typename T, typename State>
std::pair<std::shared_ptr<State>, double>
Minimax<T, State>::maxValue(const State &state, double alpha, double beta, size_type depth,
                            typename Strategy<T, State>::evalf eval) const {
    if (state.is_leaf() || depth >= max_depth) {
        return {std::make_shared<State>(state), eval(state, depth)};
    }
    double v = NINF;
    std::shared_ptr<State> principal_var = nullptr;
    for (const auto &move : state.possible_moves()) {
        auto child_node = state.peek_update(move);
        auto max_val = minValue(child_node, alpha, beta, depth + 1, eval);
        if (max_val.second > v) {
            principal_var = max_val.first;
            v = max_val.second;
        }
        if (v >= beta) {
            return {principal_var, v};
        }
        alpha = std::max(alpha, v);
    }
    return {principal_var, v};
}

template<typename T, typename State>
std::pair<std::shared_ptr<State>, double>
Minimax<T, State>::minValue(const State &state, double alpha, double beta, size_type depth,
                            typename Strategy<T, State>::evalf eval) const {
    if (state.is_leaf() || depth >= max_depth) {
        return {std::make_shared<State>(state), eval(state, depth)};
    }
    double v = INF;
    std::shared_ptr<State> principal_var = nullptr;
    for (const auto &move : state.possible_moves()) {
        auto child_node = state.peek_update(move);
        auto min_val = maxValue(child_node, alpha, beta, depth + 1, eval);
        if (min_val.second < v) {
            v = min_val.second;
            principal_var = min_val.first;
        }
        if (v <= alpha) {
            return {principal_var, v};
        }
        beta = std::min(beta, v);
    }
    return {principal_var, v};
}


#endif //SLIDER_MINIMAX_H
