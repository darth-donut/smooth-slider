//
// @author: jiahong
// @date  : 7/12/17 3:53 AM
//

#ifndef SLIDER_SLIDERGUI_H
#define SLIDER_SLIDERGUI_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "slider/Slider.h"
#include "Board.h"

#define BOX_SIZE 50
#define WIDTH_PAD 0.3f
#define HEIGHT_PAD 0.17f
#define ERR_TEXT_SIZE 28

class SliderGUI : public Slider {
public:
    SliderGUI(SliderPlayer agent,
              size_t size,
              SliderPlayer player, const sf::RenderWindow&);

    Move next_move() override;
    bool ready_to_move() override { return ready; };

private:
    const sf::RenderWindow& window;
    Move::Coordinate mouse_clk_pos;
    bool accept_mouse_response = false;
    bool ready = false;

private:
    inline bool player_piece_clicked(Board::size_type x, Board::size_type y) const;
};


#endif //SLIDER_SLIDERGUI_H
