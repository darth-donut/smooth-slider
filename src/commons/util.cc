//
// @author: jiahong
// @date  : 15/11/17 11:22 PM
//

#include <algorithm>
#include <cctype>

#include "util.h"

std::vector<std::string>
tokenize(const std::string &string, char delim, bool strip_ws) {
    auto string_start = string.cbegin();
    auto it = std::find(string_start, string.cend(), delim);
    std::vector<std::string> tokens;

    while (it != string.cend()) {
        while (strip_ws && isspace(*string_start)) {
            if (++string_start == string.cend()) {
                return tokens;
            }
        }
        tokens.emplace_back(string_start, it);
        string_start = it + 1;
        it = std::find(string_start, string.cend(), delim);
    }
    tokens.emplace_back(string_start, it);
    return tokens;
}

