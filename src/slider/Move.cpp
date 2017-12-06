//
// @author: jiahong
// @date  : 7/12/17 6:24 AM
//

#include <iostream>
#include "Move.h"

std::ostream &
operator<<(std::ostream &os, const Move &move) {
    std::string p = (move.player == SliderPlayer::Horizontal) ? "Hori" : "Vert";
    std::string m;
    switch (move.move) {
        case SliderMove::Up:
            m = "up";
            break;
        case SliderMove::Down:
            m = "down";
            break;
        case SliderMove::Right:
            m = "right";
            break;
        case SliderMove::Left:
            m = "left";
            break;
    }
    return os << p << " making move: " << m << " from coord: " << move.coord.first
              << ", " << move.coord.second;
}
