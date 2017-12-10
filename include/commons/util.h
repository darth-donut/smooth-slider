//
// @author: jiahong
// @date  : 15/11/17 11:22 PM
//

#ifndef SLIDER_UTIL_H
#define SLIDER_UTIL_H

#include <vector>
#include <string>
#include <cmath>
#include <regex>

#include "slider/slider_utils.h"


/// tokenizes the string according to delim
/// \param string string to be tokenized
/// \param delim delimiter. Defaults to space if not provided
/// \param strip_ws strips leading and trailing whitespaces if set to true
/// \return std::vector<std::string> of tokens
std::vector<std::string> tokenize(const std::string &string, char delim = ' ', bool strip_ws = true);

/// helper function to get the opposite player
/// \param p Player p
/// \return Opponent of player p
inline SliderPlayer other_player(SliderPlayer p);

/// translate coordinates from board's internal representation to vector indices
/// \param board_coord Board coordinates (see board.h)
/// \return vector indices corresponding to provided board coordinate
inline std::pair<size_t, size_t> board_to_vector(const std::pair<size_t, size_t> &board_coord);

/// sech^2(x) is also 1/cosh^2(x)
/// \param x domain value
/// \return doh
inline double sech2(double x);

bool is_weight(const std::string &str);

bool is_phi_func(const std::string &str);

bool is_ab_const(const std::string &str);


SliderPlayer
other_player(SliderPlayer p) {
    return p == SliderPlayer::Horizontal ? SliderPlayer::Vertical : SliderPlayer::Horizontal;
}

std::pair<size_t, size_t>
board_to_vector(const std::pair<size_t, size_t> &board_coord) {
    return std::make_pair(board_coord.second, board_coord.first);
}

double
sech2(double n) {
    return 1.0 / std::pow(std::cosh(n), 2);
}
#endif //SLIDER_UTIL_H

