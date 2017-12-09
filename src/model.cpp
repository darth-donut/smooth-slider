//
// @author: jiahong
// @date  : 9/12/17 4:53 AM
//

#include "model.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>

Model::Model(const std::string &model_file)
        : fname(model_file) {
    // todo:
    // 1: initialize phi to all K functions IN ORDER
    phi.push_back(count_eval);
    std::ifstream is(fname);
    std::istream_iterator<double> ist(is), eof;
    std::copy(ist, eof, std::back_inserter(weights));
}

void
Model::flush() {
    if (fname.size() > 0) {
        std::ofstream os(fname);
        std::copy(weights.cbegin(), weights.cend(), std::ostream_iterator<double>(os, "\n"));
        os.close();
    }
}

