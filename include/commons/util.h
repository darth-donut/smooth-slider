//
// @author: jiahong
// @date  : 15/11/17 11:22 PM
//

#ifndef SLIDER_UTIL_H
#define SLIDER_UTIL_H

#include <vector>
#include <string>


/// tokenizes the string according to delim
/// \param string string to be tokenized
/// \param delim delimiter. Defaults to space if not provided
/// \return std::vector<std::string> of tokens
std::vector<std::string> tokenize(const std::string &string, char delim = ' ');

#endif //SLIDER_UTIL_H