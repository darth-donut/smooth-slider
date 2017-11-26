#include <iostream>
#include <SFML/Graphics.hpp>

#include "slider/Slider.h"
#include "Referee.h"
#include "SliderIO.h"

int
main() {
//    sf::RenderWindow window(sf::VideoMode(200, 200), "Slider");
//    sf::CircleShape circle(100.f);
//    circle.setFillColor(sf::Color::Magenta);
//    Slider slider(7, SliderPlayer::Horizontal);
//    slider.update(Move(SliderPlayer::Horizontal, SliderMove::Right, std::make_pair(0, 0)));


//    std::cout << slider.possible_moves().size() << std::endl;
//    std::cout << slider.get_board() << std::endl;


//    while (window.isOpen()) {
//
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            switch (event.type) {
//                case sf::Event::Closed:
//                    window.close();
//                    break;
//            }
//        }
//        window.clear();
//        window.draw(circle);
//        window.display();
//    }

    constexpr std::size_t board_size = 4;

    Referee referee(std::make_shared<Slider>(board_size, SliderPlayer::Vertical),
    std::shared_ptr<Slider>(new SliderIO(board_size, SliderPlayer::Horizontal)), board_size);
    auto winner = referee.start_game();

    if (!winner.second) {       // if it wasn't a draw
        std::cout << (winner.first == SliderPlayer::Horizontal ? "Horizontal" : "Vertical") << " won!\n";
    } else {
        std::cout << "The game ended in a draw!\n";
    }

    return 0;
}