//
// @author: jiahong
// @date  : 15/11/17 11:22 PM
//

#ifndef SLIDER_UTIL_H
#define SLIDER_UTIL_H

#include <vector>
#include <string>
#include <slider/slider_utils.h>


/// tokenizes the string according to delim
/// \param string string to be tokenized
/// \param delim delimiter. Defaults to space if not provided
/// \param strip_ws strips leading and trailing whitespaces if set to true
/// \return std::vector<std::string> of tokens
std::vector<std::string> tokenize(const std::string &string, char delim = ' ', bool strip_ws = true);

// helper function to get the opposite player
inline SliderPlayer other_player(SliderPlayer p) {
    return p == SliderPlayer::Horizontal ? SliderPlayer::Vertical : SliderPlayer::Horizontal;
}


#endif //SLIDER_UTIL_H
