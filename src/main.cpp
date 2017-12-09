#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <slider/td_leaf_lambda.h>

#include "slider/gui/slider_render_window.h"
#include "slider/io/SliderIO.h"
#include "slider/gui/SliderGUI.h"
#include "slider/Referee.h"
#include "model.h"

#define SLIDER_GUI_TOGG 0

void play_games(std::size_t board_size, SliderPlayer starting_player);


int
main() {
    constexpr std::size_t board_size = 7;
    SliderPlayer starting_player = SliderPlayer::Horizontal;

#if SLIDER_GUI_TOGG
    // GUI interface
    SliderRenderWindow window(sf::VideoMode(800, 550), "Slider");
    Minimax<Move, Slider> ai_strategy{9};
    Model bob;
    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy, &bob),
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
    constexpr int ngames = 10;
    std::thread games[ngames];

    for (auto &game : games) {
        game = std::thread(play_games, board_size, starting_player);
    }
    for (auto &game : games) {
        game.join();
    }

#endif
    return 0;
}

void
play_games(std::size_t board_size, SliderPlayer starting_player) {
    Minimax<Move, Slider> ai_strategy1{4};
    Minimax<Move, Slider> ai_strategy2{5};
    Model bob(Resource::bob_model);
    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy1, &bob),
            std::make_shared<Slider>(SliderPlayer::Horizontal, board_size, starting_player, &ai_strategy2, &bob),
            board_size);
    auto winner = referee.start_game();
    if (!winner.second) {       // if it wasn't a draw
        std::cout << (winner.first == SliderPlayer::Horizontal ? "Horizontal" : "Vertical") << " won!\n";
    } else {
        std::cout << "The game ended in a draw!\n";
    }
}

