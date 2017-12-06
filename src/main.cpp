#include <iostream>
#include <SFML/Graphics.hpp>
#include <slider/io/SliderIO.h>

#include "slider/Referee.h"


void draw_board(sf::RenderWindow& window, const Referee& referee);

int
main() {

    sf::RenderWindow window(sf::VideoMode(800, 550), "Slider");

    constexpr std::size_t board_size = 7;
    Minimax<Move, Slider> ai_strategy{9};
    SliderPlayer starting_player = SliderPlayer::Horizontal;

    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy),
            std::shared_ptr<Slider>(new SliderIO(SliderPlayer::Horizontal, board_size, starting_player)),
            board_size);

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        window.clear();
        draw_board(window, referee);
        window.display();
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

void
draw_board(sf::RenderWindow& window, const Referee& referee) {
    constexpr float box_size = 50.f;
    const float width = window.getSize().x;
    const float height = window.getSize().y;
    float width_padding = width * 3.0f/10;
    float height_padding = height * 1.0f/10;
    float x = width_padding;
    float y = height_padding;

    for (size_t i = 0; i != referee.get_board().size(); ++i) {
        for (size_t j = 0; j != referee.get_board().size(); ++j) {
            sf::Vector2f pos(x + i * box_size, y + j * box_size);
            sf::RectangleShape box(sf::Vector2f(box_size, box_size));
            box.setPosition(pos);
            if ((j + i) % 2) {
                box.setFillColor(sf::Color::Green);
            } else {
                box.setFillColor(sf::Color::White);
            }
            window.draw(box);

            // draw board piece if there is one (incl blocks)
            sf::CircleShape piece(box_size/3, 7);
            piece.setOrigin(box_size/3, box_size/3);
            switch (referee.get_board()[j][i]) {
                case SliderPiece::Horizontal:
                    piece.setFillColor(sf::Color::Magenta);
                    break;
                case SliderPiece::Vertical:
                    piece.setFillColor(sf::Color::Cyan);
                    break;
                case SliderPiece::Block:
                    piece.setPointCount(4); // make square instead
                    piece.setFillColor(sf::Color::Black);
                    break;
            }
            if (referee.get_board()[j][i] != SliderPiece::Blank) {
                sf::Vector2f piece_pos(pos.x + box_size/2, pos.y + box_size/2);
                piece.setPosition(piece_pos);
                window.draw(piece);
            }
        }
    }
}