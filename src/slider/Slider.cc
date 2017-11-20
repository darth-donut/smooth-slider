//
// @author: jiahong
// @date  : 14/11/17 10:00 PM
//

#include "Slider.h"


Slider::Slider(std::size_t size)
        : size(size), board(size) {}

bool Slider::update(const Move &move) {
    return false;
}

Slider Slider::peek_update(const Move &move) const {
    return Slider(0);
}

std::vector<Move> Slider::possible_moves() const {
    return std::vector<Move>();
}

bool Slider::is_leaf() const {
    return false;
}

