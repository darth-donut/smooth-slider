//
// @author: jiahong
// @date  : 9/12/17 4:53 AM
//

#ifndef SLIDER_MODEL_H
#define SLIDER_MODEL_H

#include <vector>
#include <string>
#include <slider/evaluation_functions/basic_eval.h>

class Slider;

class Model {
public:
    typedef double evalf(const Slider &, std::size_t);

    typedef std::vector<double>::size_type size_type;
public:
    Model() : phi({count_eval}), weights({1}) {}

    Model(const std::string &model_file);

    double &operator[](size_type i) { return weights[i]; };

    size_type size() const { return weights.size(); }

    void flush();

public:
    std::vector<evalf *> phi;
    static constexpr double a = 1;
    static constexpr double b = 1;
private:
    std::vector<double> weights;
    const std::string fname;
};


#endif //SLIDER_MODEL_H
