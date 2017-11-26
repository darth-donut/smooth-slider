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
/// Type constraints on T requires a default constructor,
/// Type constraints on State requires state::possible_moves(), state::peek_update(const T&), state::is_leaf()
/// \tparam T Type move. Returns this move as the best move to make given a current state
/// \tparam State Type State. State requires the following to be defined
/// <br>
/// - possible_moves() - returns a vector of moves (of type T) that can be made legally from a given state
/// <br>
/// - peek_update() - return a new state after applying the provided move.
/// <br>
/// - is_leaf() - returns true if this state is a leaf state, false otherwise
template <typename T, typename State>
class Minimax : public Strategy<T, State> {
public:
    typedef size_t size_type;
    Minimax() = default;
    explicit Minimax(size_type max_depth) : max_depth(max_depth) {}
    std::pair<T, bool> next_move(const State&, double (*eval) (const State&)) const override;

private:
    static constexpr double INF  = std::numeric_limits<double>::max();
    static constexpr double NINF = std::numeric_limits<double>::min();
    size_type max_depth = std::numeric_limits<size_type>::max();
private:
    double maxValue(const State &state, double alpha, double beta, size_type depth ,double (*eval)(const State&)) const;
    double minValue(const State &state, double alpha, double beta, size_type depth ,double (*eval)(const State&)) const;
};

template <typename T, typename State>
std::pair<T, bool>
Minimax<T, State>::next_move(const State &state, double (*eval)(const State &)) const {
    double v = NINF;
    double alpha = NINF;
    double beta = INF;
    auto possible_moves = state.possible_moves();
    T best_move;
    if (possible_moves.size() == 0) {
        return std::make_pair(best_move, false);
    }

    for (const auto &move : possible_moves) {
        double v_prime = minValue(state.peek_update(move), alpha, beta, 1, eval);
        if (v_prime >= v) {
            v = v_prime;
            best_move = move;
        }
        alpha = std::max(v, alpha);
    }
    return std::make_pair(best_move, true);
}

template <typename T, typename State>
double
Minimax<T, State>::maxValue(const State &state, double alpha, double beta, size_type depth, double (*eval)(const State &)) const {
    if (state.is_leaf() || depth >= max_depth) {
        return eval(state);
    }

    double v = NINF;
    for (const auto &move : state.possible_moves()) {
        v = std::max(v, minValue(state.peek_update(move), alpha, beta, depth + 1, eval));
        if (v >= beta) {
            return v;
        }
        alpha = std::max(alpha, v);
    }
    return v;
}

template <typename T, typename State>
double
Minimax<T, State>::minValue(const State &state, double alpha, double beta, size_type depth, double (*eval)(const State &)) const {
    if (state.is_leaf() || depth >= max_depth) {
        return eval(state);
    }

    double v = INF;
    for (const auto &move : state.possible_moves()) {
        v = std::min(v, maxValue(state.peek_update(move), alpha, beta, depth + 1, eval));
        if (v <= alpha) {
            return v;
        }
        beta = std::min(beta, v);
    }
    return v;
}


#endif //SLIDER_MINIMAX_H
