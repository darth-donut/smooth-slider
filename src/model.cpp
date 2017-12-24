//
// @author: jiahong
// @date  : 9/12/17 4:53 AM
//

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cassert>

#include "model.h"

Model::Model(const std::string &model_file)
        : fname(model_file) {
    std::ifstream is(fname);
    // 1: initialize weights from all K weights in model file

    // 2: initialize phi to all K functions in model file
    // 3: initialize a and b values
    std::string buffer;
    while (getline(is, buffer)) {
        if (is_weight(buffer)) {
            weights.push_back(std::stod(buffer));
        } else if (is_phi_func(buffer)) {
            // unless someone tempered with our model file, or if it's corrupted, it should always be in the hash-map
            assert(eval_dict.find(buffer) != eval_dict.cend());
            if (eval_dict.find(buffer) != eval_dict.cend()) {
                phi.push_back(eval_dict[buffer]);
                phis.push_back(buffer);
            } else {
                throw std::domain_error("No such evaluation function!");
            }
        } else if (is_ab_const(buffer)) {
            auto index = buffer.find_first_of(':');
            assert(index != std::string::npos);
            a = std::stod(buffer.substr(index + 1));
            if (getline(is, buffer)) {
                assert(buffer.find_first_of("b:") != std::string::npos);
                index = buffer.find_first_of(':');
                b = std::stod(buffer.substr(index + 1));
            }
        } else {
            throw std::domain_error("unknown sequence: " + buffer);
        }
    }

}

void
Model::flush() const {
    if (!fname.empty()) {

        std::ofstream os(fname);

        // use fixed (non-scientific) notation
        os << std::fixed;

        // 1. refill updated weights
        std::copy(weights.cbegin(), weights.cend(), std::ostream_iterator<double>(os, "\n"));

        // 2. refill phi functions
        std::copy(phis.cbegin(), phis.cend(), std::ostream_iterator<std::string>(os, "\n"));

        // 3. refill a and b values
        os << "a:" << a << '\n' << "b:" << b << '\n';
        os.close();
    }

}

