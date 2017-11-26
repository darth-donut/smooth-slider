//
// @author: jiahong
// @date  : 26/11/17 6:57 PM
//

#ifndef SLIDER_REFEREE_H
#define SLIDER_REFEREE_H


#include <utility>
#include <memory>

#include "Slider.h"

class Referee {
public:
    Referee(std::shared_ptr<Slider> p1, std::shared_ptr<Slider> p2, Board::size_type n)
            : p1(std::move(p1)),
              p2(std::move(p2)),
              slider_board(n) { sanity_check(); }

    /// returns the winner of this slider match. If it's a draw, value in start_game().first is undefined.
    /// Use start_game().second to determine if it's a draw (value will be true if it's a draw, false otherwise)
    /// eg. (SliderPlayer::Horizontal, true) => draw
    ///     (SliderPlayer::Horizontal, false) => Horizontal won the game
    /// \return
    std::pair<SliderPlayer, bool> start_game();

private:
    std::shared_ptr<Slider> p1;
    std::shared_ptr<Slider> p2;
    Board slider_board;

private:
    /// basic sanity checking
    void sanity_check() const;
    std::pair<std::shared_ptr<Slider>, std::shared_ptr<Slider>> get_players() const;
    bool draw_game() const;
};


#endif //SLIDER_REFEREE_H