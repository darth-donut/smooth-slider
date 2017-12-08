#include <iostream>
#include <SFML/Graphics.hpp>
#include <slider/io/SliderIO.h>
#include <slider/gui/SliderGUI.h>
#include <slider/gui/slider_render_window.h>

#include "slider/Referee.h"

#define SLIDER_GUI_TOGG 1

int
main() {
    constexpr std::size_t board_size = 7;
    SliderPlayer starting_player = SliderPlayer::Horizontal;

#if SLIDER_GUI_TOGG
    // GUI interface
    SliderRenderWindow window(sf::VideoMode(800, 550), "Slider");
    Minimax<Move, Slider> ai_strategy{9};

    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy),
            std::shared_ptr<Slider>(new SliderGUI(SliderPlayer::Horizontal, board_size, starting_player, window)),
            board_size,
            &window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        referee.update();
    }
#else
    // CMD interface

    // Battle between 2 AI - use std::make_shared<Slider>(new SliderIO ... )
    // for human CMD interaction instead
    Minimax<Move, Slider> ai_strategy1{7};
    Minimax<Move, Slider> ai_strategy2{6};
    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy1),
            std::make_shared<Slider>(SliderPlayer::Horizontal, board_size, starting_player, &ai_strategy2),
            board_size);

    auto winner = referee.start_game(true);

    if (!winner.second) {       // if it wasn't a draw
        std::cout << (winner.first == SliderPlayer::Horizontal ? "Horizontal" : "Vertical") << " won!\n";
    } else {
        std::cout << "The game ended in a draw!\n";
    }

#endif
    return 0;
}
