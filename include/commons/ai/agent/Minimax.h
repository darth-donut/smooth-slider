//
// @author: jiahong
// @date  : 15/11/17 12:14 AM
//

#ifndef SLIDER_MINIMAX_H
#define SLIDER_MINIMAX_H

#include <vector>
#include <limits>
#include <tuple>
#include <cmath>
#include <cassert>

#include "Strategy.h"
#include "Move.h"

#define EPS 1e-6

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
    static constexpr size_t STATE_INDEX = 0;
    static constexpr size_t V_INDEX = 1;
    static constexpr size_t DEPTH_INDEX = 2;
public:
    Minimax() = default;

    explicit Minimax(size_type max_depth) : max_depth(max_depth) {}

    std::pair<Move, bool> next_move(const State &, typename Strategy<Move, State>::evalf) const override;

private:
    static constexpr double INF = std::numeric_limits<double>::max();
    static constexpr double NINF = std::numeric_limits<double>::lowest();
    size_type max_depth = std::numeric_limits<size_type>::max();
private:
    std::tuple<std::shared_ptr<State>, double, size_type>
    maxValue(const State &state, double alpha, double beta, size_type depth,
             typename Strategy<Move, State>::evalf) const;

    std::tuple<std::shared_ptr<State>, double, size_type>
    minValue(const State &state, double alpha, double beta, size_type depth,
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
    size_type principal_depth;
    for (const auto &move : possible_moves) {
        auto ret_val = minValue(state.peek_update(move), alpha, beta, 1, eval);
        double v_prime = std::get<V_INDEX>(ret_val);
        if (v_prime > v) {
            principal_var = std::get<STATE_INDEX>(ret_val);
            principal_depth = std::get<DEPTH_INDEX>(ret_val);
            v = v_prime;
            best_move = move;
        }
        alpha = std::max(v, alpha);
    }
    best_move.add_metadata(std::make_tuple(principal_var, v, principal_depth));
    return std::make_pair(best_move, true);
}

template<typename T, typename State>
std::tuple<std::shared_ptr<State>, double, typename Minimax<T, State>::size_type>
Minimax<T, State>::maxValue(const State &state, double alpha, double beta, size_type depth,
                            typename Strategy<T, State>::evalf eval) const {
    if (state.is_leaf() || depth >= max_depth) {
        return std::make_tuple(std::make_shared<State>(state), eval(state, depth), depth);
    }
    double v = NINF;
    std::shared_ptr<State> principal_var = nullptr;
    size_type principal_depth = depth;
    for (const auto &move : state.possible_moves()) {
        auto child_node = state.peek_update(move);
        auto max_val = minValue(child_node, alpha, beta, depth + 1, eval);
        // update condition:
        // 1. obviously, when principal_var's evalf score is > current v
        // 2. when they are 'equal (within eps)' and the depth is smaller (i.e. faster to victory)
        if (std::get<V_INDEX>(max_val) > v ||
            ((std::abs(std::get<V_INDEX>(max_val) - v) < EPS) && std::get<DEPTH_INDEX>(max_val) < principal_depth)) {
            v = std::get<V_INDEX>(max_val);
            principal_var = std::get<STATE_INDEX>(max_val);
            principal_depth = std::get<DEPTH_INDEX>(max_val);
        }
        if (v >= beta) {
            return std::make_tuple(principal_var, v, principal_depth);
        }
        alpha = std::max(alpha, v);
    }
    assert(principal_var != nullptr);
    return std::make_tuple(principal_var, v, principal_depth);
}

template<typename T, typename State>
std::tuple<std::shared_ptr<State>, double, typename Minimax<T, State>::size_type>
Minimax<T, State>::minValue(const State &state, double alpha, double beta, size_type depth,
                            typename Strategy<T, State>::evalf eval) const {
    if (state.is_leaf() || depth >= max_depth) {
        return std::make_tuple(std::make_shared<State>(state), eval(state, depth), depth);
    }
    double v = INF;
    std::shared_ptr<State> principal_var = nullptr;
    size_type principal_depth = depth;
    // update condition:
    // 1. obviously, when principal_var's evalf score is < current v
    // 2. when they are 'equal (within eps)' and the depth is greater (i.e. dilly dally in losing the game)
    for (const auto &move : state.possible_moves()) {
        auto child_node = state.peek_update(move);
        auto min_val = maxValue(child_node, alpha, beta, depth + 1, eval);
        if (std::get<V_INDEX>(min_val) < v ||
            ((std::abs(std::get<V_INDEX>(min_val) - v) < EPS) && std::get<DEPTH_INDEX>(min_val) > principal_depth)) {
            v = std::get<V_INDEX>(min_val);
            principal_var = std::get<STATE_INDEX>(min_val);
            principal_depth = std::get<DEPTH_INDEX>(min_val);
        }
        if (v <= alpha) {
            return std::make_tuple(principal_var, v, principal_depth);
        }
        beta = std::min(beta, v);
    }
    assert(principal_var != nullptr);
    return std::make_tuple(principal_var, v, principal_depth);
}


#endif //SLIDER_MINIMAX_H
