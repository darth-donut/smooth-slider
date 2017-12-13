#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <slider/td_leaf_lambda.h>
#include <learner/Trainer.h>

#include "slider/gui/slider_render_window.h"
#include "slider/io/SliderIO.h"
#include "slider/gui/SliderGUI.h"
#include "slider/Referee.h"
#include "model.h"

#define SLIDER_GUI_TOGG 1

void play_games(std::size_t board_size, SliderPlayer starting_player);


int
main() {
    constexpr std::size_t board_size = 4;
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
    Trainer train_model(board_size, starting_player, 1);
    train_model.begin_training();
#endif
    return 0;
}

