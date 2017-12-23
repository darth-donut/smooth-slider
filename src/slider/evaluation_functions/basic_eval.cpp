//
// @author: jiahong
// @date  : 27/11/17 11:53 PM
//

#include <cmath>
#include <queue>
#include <unordered_set>
#include <tuple>

#include "basic_eval.h"
#include "model.h"
#include "Move.h"
#include "util.h"
#include "Board.h"

#define BLOCK_PENALTY_MULTIPLIER 3
#define BLOCK_PENALTY(n) ((n) * (BLOCK_PENALTY_MULTIPLIER))


inline static double f(double a, double b, double score);
/// calculates the distance needed for all agent's pieces to move towards the edge of the board (to win).
/// Ignores enemy movement when doing so. When a piece has NO MOVE AT ALL to make (i.e. completely surrounded),
/// that piece incurrs a penalty score of board size * 3 (arbitrary). Therefore, the returned value from this
/// function cannot be assumed to be the ABSOLUTE CORRECT manhattan distance when there's a complete block at least
/// on one of the pieces.
/// \param state current state
/// \param agent agent to calculate the distance for
/// \return (true) distance (if there's no pieces with totally blocked path)
static double calc_manhattan_dist(const Slider& state, SliderPlayer agent);
inline static double normalize_manhanntan(const Slider &slider, double raw_score);
inline static bool is_draw(const Slider &state);


double
pieces_left(const Slider &state, size_t) {
    auto agent = state.get_agent();
    double pieces_left = state.get_board().get_piece_positions(agent).size();
    double opponent_left = state.get_board().get_piece_positions(other_player(agent)).size();
    // the LESS pieces left, the better. (the MORE enemy left the better)
    return opponent_left - pieces_left;
}

double
enemy_move_count(const Slider &state, size_t) {
    auto opponent = other_player(state.get_agent());
    double enemy_pieces_left = state.get_board().get_piece_positions(opponent).size();
    // each piece has only a max of 3 possible moves at any time.
    // (maybe lower, but never higher)
    return (enemy_pieces_left * 3) - state.opponent_possible_moves().size();
}

double
player_move_count(const Slider &state, size_t) {
    return state.possible_moves().size();
}

double
manhattan_dist(const Slider &state, size_t) {
    auto agent = state.get_agent();
    double agent_distance = normalize_manhanntan(state, calc_manhattan_dist(state, agent));
    double opponent_distance = normalize_manhanntan(state, calc_manhattan_dist(state, other_player(agent)));
    return opponent_distance - agent_distance;
}

double
enemy_blocked_pieces(const Slider &state, size_t) {
    auto enemy = other_player(state.get_agent());
    auto enemy_pieces_coord = state
            .get_board()
            .get_piece_positions(enemy);
    int blocked = 0;
    for (const auto &coord : enemy_pieces_coord) {
        for (const auto &move : Slider::moveset) {
            if ((enemy == SliderPlayer::Vertical && move == SliderMove::Down) ||
                (enemy == SliderPlayer::Horizontal && move == SliderMove::Left)) {
                // these move-sets aren't allowed!
                continue;
            }
            Move possible_move(enemy, move, coord);
            if (state.get_board().is_legal(possible_move)) {
                break;
            } else {
                // if even the last move is not legal, we have a piece that's totally blocked
                blocked += (move == Slider::moveset.back());
            }
        }
    }
    return blocked;
}

double
unblockable_path(const Slider &state, size_t) {
    // todo
    return 0;
}

double
straight_path_block(const Slider &state, size_t depth) {
    size_t blockage = 0;
    auto enemy = other_player(state.get_agent());
    auto horizontal = enemy == SliderPlayer::Horizontal;
    for (const auto &coord : state.get_board().get_piece_positions(enemy)) {
        auto converted_coord = board_to_vector(coord);
        if (horizontal) {
            for (auto x = converted_coord.first; x < state.get_board().size(); ++x) {
                auto path_piece = state.get_board()[x][converted_coord.second];
                blockage += (path_piece != SliderPiece::Blank);
            }
        } else {
            for (auto y = converted_coord.second + 1; y > 0; --y) {
                auto path_piece = state.get_board()[converted_coord.first][y - 1];
                blockage += (path_piece != SliderPiece::Blank);
            }
        }
    }
    return blockage;
}


double
evaluate(const Slider &state, size_t depth) {
    Model *model = state.get_model();
    double score = 0;
    double final_score = score;
    if (!is_draw(state)) {
        for (auto i = 0; i < model->size(); ++i) {
            // function * weight
            score += (model->phi[i](state, depth) * (*model)[i]);
        }
        double raw_score = transform_score(score, depth);
        // apply tanh function with a and b parameters
        final_score = f(model->a, model->b, raw_score);
        // if it was dead even, the upper hand goes to the one with the current hand
        if (final_score == 0) {
            final_score = (state.get_agent() == state.get_player()) ? 1e-6 : -1e-6;
        }
    }   // else, final_score is 0, indicating a draw game

    return final_score;
}

static bool
is_draw(const Slider &state) {
    return state.possible_moves().empty() && state.opponent_possible_moves().empty();
}



// todo:
// 1) decrease total block penalty (will prefer to block over winning!)
// 2) bias multiplier when enemy dist > 0 but agent_dist = 0 (i.e. we won) ==> should score a little more (biasness)
// 3) use simplified manhattan distance instead -> we're always going straight as the shortest path anyway (unless obstacle)
static double
calc_manhattan_dist(const Slider& state, SliderPlayer agent) {
    double distance = 0;
    const auto all_coords = state.get_board().get_piece_positions(agent);

    for (const auto &start_node : all_coords) {
        std::queue<std::tuple<Move::Coordinate, size_t, Board>> queue;
        std::unordered_set<Move::Coordinate> visited;
        // the starting node always has distance 0 from start
        queue.push(std::make_tuple(start_node, 0, state.get_board()));
        bool finish = false;

        // start bfs on start_node as starting point
        while (!queue.empty() && !finish) {
            const auto node = queue.front();
            queue.pop();
            visited.insert(std::get<0>(node));
            unsigned char declined = 0;

            // for each neighbour to node:
            for (const auto &move : Slider::moveset) {
                if ((agent == SliderPlayer::Vertical && move == SliderMove::Down) ||
                    (agent == SliderPlayer::Horizontal && move == SliderMove::Left)) {
                    // these move-sets aren't allowed!
                    continue;
                }
                Move possible_move(agent, move, std::get<0>(node));
                // if the move is legal (i.e. it's node's current neighbour node), add the new
                // coordinate to queue ONLY IF it's not been visited before
                if (std::get<2>(node).is_legal(possible_move)) {
                    // if we've reached the destination node, add the total tallied distance to "distance"
                    if (std::get<2>(node).is_edge_move(possible_move)) {
                        distance += (std::get<1>(node) + 1);
                        finish = true;
                        break;
                    }
                    auto new_coord = possible_move.apply_move();
                    if (visited.find(new_coord) == visited.cend()) {
                        // neighbouring nodes have distance node.second + 1 (current depth + 1)
                        Board new_board(std::get<2>(node));
                        new_board.make_move(possible_move);     // already confirmed that the move is legal
                        queue.push(std::make_tuple(new_coord, std::get<1>(node) + 1, std::move(new_board)));
                    }
                } else {
                    if (++declined >= 3) {
                        // penalty - this piece is completely blocked
                        distance += BLOCK_PENALTY(state.get_board().size());
                    }
                }
            }
        }
    }
    return distance;
}

static double
normalize_manhanntan(const Slider &slider, double raw_score) {
    auto board_size = slider.get_board().size();
    auto pieces = board_size - 1;
    double worst_case = pieces * BLOCK_PENALTY(board_size);
    return raw_score / worst_case;
}

static double
f(double a, double b, double score) {
    return a * std::tanh(b * score);
}
