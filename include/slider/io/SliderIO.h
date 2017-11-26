//
// @author: jiahong
// @date  : 26/11/17 8:33 PM
//

#ifndef SLIDER_SLIDERIO_H
#define SLIDER_SLIDERIO_H

#include <string>

#include "Slider.h"


/// Slider IO is a kind of Slider, wherein the method next_move() is overridden to taking input from I/O,
/// everything else is derived from Slider itself.
class SliderIO : public Slider {
public:
    SliderIO(size_t size, SliderPlayer player)
        : Slider(size, player) {}

    Move next_move() override;

private:
    /// a string representation of a decorated board (i.e. edges marked with alphanumerics as its coordinate system)
    /// \return String representation (wrapped board with alphanumerics as its coordinates)
    std::string decorated_board() const;

};


#endif //SLIDER_SLIDERIO_H
