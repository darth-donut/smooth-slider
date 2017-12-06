#include <iostream>
#include <SFML/Graphics.hpp>
#include <slider/io/SliderIO.h>
#include <slider/gui/SliderGUI.h>

#include "slider/Referee.h"


int
main() {

    sf::RenderWindow window(sf::VideoMode(800, 550), "Slider");

    constexpr std::size_t board_size = 7;
    Minimax<Move, Slider> ai_strategy{9};
    SliderPlayer starting_player = SliderPlayer::Horizontal;

    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy),
            std::shared_ptr<Slider>(new SliderGUI(SliderPlayer::Horizontal, board_size, starting_player, window)),
            board_size,
            &window);

//    auto winner = referee.start_game();
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


    // 2 ai battle
//    Minimax<Move, Slider> ai_strategy2{9};
//    Referee referee(
//            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy),
//            std::make_shared<Slider>(SliderPlayer::Horizontal, board_size, starting_player, &ai_strategy2),
//            board_size);
//
//    auto winner = referee.start_game();
//
//    if (!winner.second) {       // if it wasn't a draw
//        std::cout << (winner.first == SliderPlayer::Horizontal ? "Horizontal" : "Vertical") << " won!\n";
//    } else {
//        std::cout << "The game ended in a draw!\n";
//    }

    return 0;
}
