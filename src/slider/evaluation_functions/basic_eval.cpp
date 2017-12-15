//
// @author: jiahong
// @date  : 27/11/17 11:53 PM
//

#include <cmath>
#include <queue>
#include <unordered_set>

#include "basic_eval.h"
#include "model.h"
#include "Move.h"
#include "util.h"

static double f(double a, double b, double score);

static double
f(double a, double b, double score) {
    return a * std::tanh(b * score);
}

double
pieces_left(const Slider &state, size_t) {
    auto agent = state.get_agent();
    auto pieces_left = state.get_board().get_piece_positions(agent).size();
    // the LESS pieces left, the better.
    return (state.get_board().size() - 1) - pieces_left;
}

double
enemy_move_count(const Slider &state, size_t) {
    auto opponent = other_player(state.get_agent());
    auto enemy_pieces_left = state.get_board().get_piece_positions(opponent).size();
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
    double distance = 0;
    auto agent = state.get_agent();
    const auto all_coords = state.get_board().get_piece_positions(agent);

    for (const auto &start_node : all_coords) {
        auto player_coordinates = std::queue<std::pair<Move::Coordinate, size_t>>();
        std::unordered_set<Move::Coordinate> visited;
        // the starting node always has distance 0 from start
        player_coordinates.push({start_node, 0});

        // start bfs on start_node as starting point
        while (!player_coordinates.empty()) {
            const auto node = player_coordinates.front();
            player_coordinates.pop();
            visited.insert(node.first);
            // for each neighbour to node:
            for (const auto &move : Slider::moveset) {
                if ((agent == SliderPlayer::Vertical && move == SliderMove::Down) ||
                    (agent == SliderPlayer::Horizontal && move == SliderMove::Left)) {
                    // these move-sets aren't allowed!
                    continue;
                }
                Move possible_move(agent, move, node.first);
                // if the move is legal (i.e. it's node's current neighbour node), add the new
                // coordinate to player_coordinates ONLY IF it's not been visited before
                if (state.get_board().is_legal(possible_move)) {
                    // if we've reached the destination node, add the total tallied distance to "distance"
                    if (state.get_board().is_edge_move(possible_move)) {
                        distance += (node.second + 1);
                        break;
                    }
                    auto new_coord = possible_move.apply_move();
                    if (visited.find(new_coord) == visited.cend()) {
                        // neighbouring nodes have distance node.second + 1 (current depth + 1)
                        player_coordinates.push({new_coord, node.second + 1});
                    }
                }
            }
        }

    }
    return -distance;
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
    for (auto i = 0; i < model->size(); ++i) {
        // function * weight
        score += (model->phi[i](state, depth) * (*model)[i]);
    }
    score *= (1 - (depth * 1.0) / (depth + 1));
    return f(state.get_model()->a, state.get_model()->b, score);
}
