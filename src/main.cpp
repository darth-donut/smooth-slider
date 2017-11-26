#include <iostream>
#include <SFML/Graphics.hpp>

#include "slider/Slider.h"

int
main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "Slider");
    sf::CircleShape circle(100.f);
    circle.setFillColor(sf::Color::Magenta);
    Slider slider(7, SliderPlayer::Horizontal);
//    slider.update(Move(SliderPlayer::Horizontal, SliderMove::Right, std::make_pair(0, 0)));


//    std::cout << slider.possible_moves().size() << std::endl;
//    std::cout << slider.get_board() << std::endl;


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
        window.draw(circle);
        window.display();
    }
    return 0;
}