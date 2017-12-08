//
// @author: jiahong
// @date  : 9/12/17 4:53 AM
//

#ifndef SLIDER_MODEL_H
#define SLIDER_MODEL_H

#include <vector>

class Model {
public:
    typedef std::vector<double>::size_type size_type;

    // todo the i'th weight of this model (1 .. K) where K = number of total weights (this is eq to # of functions)
    double& operator[](size_type i);

    size_type size() const { return weights.size(); }

private:
    std::vector<double> weights;
};


#endif //SLIDER_MODEL_H
