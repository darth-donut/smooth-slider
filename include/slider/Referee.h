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

    Referee(std::shared_ptr<Slider> p1, std::shared_ptr<Slider> p2, Board::size_type n, sf::RenderWindow *window)
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

    /// This method is called instead of update if the players aren't using GUI interface. It's a blocking method
    /// that will only return after the match is over - i.e. returns the winner.
    /// returns the winner of this slider match. If it's a draw, value in start_game().first is undefined.
    /// Use start_game().second to determine if it's a draw (value will be true if it's a draw, false otherwise)
    /// eg. (SliderPlayer::Horizontal, true) => draw
    ///     (SliderPlayer::Horizontal, false) => Horizontal won the game
    /// \param disp_interm true if the board should be displayed between each intermediate move (don't use this if
    ///                    one of the agent is SliderIO because SliderIO prints the board when querying for a move
    ///                    already)
    /// \return a pair consisting of SliderPlayer that won and bool to indicate that the game ended in a draw, in which
    ///         the value of SliderPlayer will have no meaning whatsoever.
    std::pair<SliderPlayer, bool> start_game(bool disp_interm = false);

    /// This update is analogous to start_game() but with GUI instead. It updates the window display
    /// passed into the 2nd constructor of Referee class (sf::RenderWindow). it does not wait for the entire
    /// game to finish, but rather updates in a per-frame and per-move basis.
    /// XXX: INSTRUCTIONS: call referee.update() in while (window.isOpen()) loop of SFML to render the board in window
    void update();

private:
    std::shared_ptr<Slider> p1;
    std::shared_ptr<Slider> p2;
    Board slider_board;
    sf::RenderWindow *window = nullptr;
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