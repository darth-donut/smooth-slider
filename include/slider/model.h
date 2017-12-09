//
// @author: jiahong
// @date  : 9/12/17 4:53 AM
//

#ifndef SLIDER_MODEL_H
#define SLIDER_MODEL_H

#include <vector>

class Slider;

class Model {
public:
    typedef double evalf (const Slider&, std::size_t);
    typedef std::vector<double>::size_type size_type;

    // todo the i'th weight of this model (1 .. K) where K = number of total weights (this is eq to # of functions)
    // todo: need to make this thread safe (there are consumers(slider) and producers(TDLeafLambda) using this)
    double& operator[](size_type i) { return weights[i]; };

    size_type size() const { return weights.size(); }

public:
    std::vector<evalf*> phi;

private:
    std::vector<double> weights;
};


#endif //SLIDER_MODEL_H
