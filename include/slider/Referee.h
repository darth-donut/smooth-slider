//
// @author: jiahong
// @date  : 26/11/17 6:57 PM
//

#ifndef SLIDER_REFEREE_H
#define SLIDER_REFEREE_H


#include <utility>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Slider.h"

class Referee {
public:
    Referee(std::shared_ptr<Slider> p1, std::shared_ptr<Slider> p2, Board::size_type n)
            : p1(std::move(p1)),
              p2(std::move(p2)),
              slider_board(n) { sanity_check(); }

    Referee(std::shared_ptr<Slider> p1, std::shared_ptr<Slider> p2, Board::size_type n, sf::RenderWindow* window)
            : p1(std::move(p1)),
              p2(std::move(p2)),
              slider_board(n),
              window(window) {
        sanity_check();
        // current_player is the player that has to make the move this round
        // other_player is the 'other" player that made its move in the previous round
        auto tmp = get_players();
        current_player = tmp.first;
        other_player = tmp.second;
    }

    /// returns the winner of this slider match. If it's a draw, value in start_game().first is undefined.
    /// Use start_game().second to determine if it's a draw (value will be true if it's a draw, false otherwise)
    /// eg. (SliderPlayer::Horizontal, true) => draw
    ///     (SliderPlayer::Horizontal, false) => Horizontal won the game
    /// \return
    std::pair<SliderPlayer, bool> start_game();

    /// returns a const reference to internal board maintained by referee
    /// \return Const ref to class Board
    const Board& get_board() const { return slider_board; }

    void update();

private:
    std::shared_ptr<Slider> p1;
    std::shared_ptr<Slider> p2;
    Board slider_board;
    sf::RenderWindow* window = nullptr;
    std::shared_ptr<Slider> current_player;
    std::shared_ptr<Slider> other_player;
private:
    /// basic sanity checking
    void sanity_check() const;
    std::pair<std::shared_ptr<Slider>, std::shared_ptr<Slider>> get_players() const;
    bool draw_game() const;
    void draw_gui();
};


#endif //SLIDER_REFEREE_H