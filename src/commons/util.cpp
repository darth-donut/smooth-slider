//
// @author: jiahong
// @date  : 15/11/17 11:22 PM
//

#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <cmath>
#include <regex>

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


bool
is_weight(const std::string &str) {
    // any digit, optionally followed by a decimal point and one or more digit after the d.p.
    static std::regex weight_ptn(R"(^-?\d+(?:\.\d*)?$)");
    return std::regex_match(str, weight_ptn);
}

bool
is_phi_func(const std::string &str) {
    // any word with length >= 1
    static std::regex phi_ptn(R"(^\w+$)");
    return std::regex_match(str, phi_ptn);
}

bool
is_ab_const(const std::string &str) {
    // a: or b: followed by any digit,
    // optionally followed by a decimal point and one or more digit after the d.p.
    static std::regex ab_ptn(R"(^[ab]:-?\d+(?:\.\d*)?$)");
    return std::regex_match(str, ab_ptn);
}
