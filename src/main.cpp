#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>
#include <slider/td_leaf_lambda.h>
#include <learner/Trainer.h>

#include "agents/gui/slider_render_window.h"
#include "agents/io/SliderIO.h"
#include "agents/gui/SliderGUI.h"
#include "slider/Referee.h"
#include "model.h"

#define SLIDER_GUI_TOGG 0



int
main() {
    constexpr std::size_t board_size = 5;
    SliderPlayer starting_player = SliderPlayer::Horizontal;

#if SLIDER_GUI_TOGG
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; // 8x AA
//     GUI interface
    SliderRenderWindow window(sf::VideoMode(800, 550), "Slider", sf::Style::Titlebar | sf::Style::Close, settings);
    Minimax<Move, Slider> ai_strategy{7};
    Model bob(Resource::bob_model);

    // board setup
    Board board(board_size);

    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board, starting_player, &ai_strategy, &bob),
            std::shared_ptr<Slider>(new SliderGUI(SliderPlayer::Horizontal, board, starting_player, window)),
            board,
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
    Trainer train_model(board_size, starting_player, 100);
    train_model.begin_training();
#endif
    return 0;
}

