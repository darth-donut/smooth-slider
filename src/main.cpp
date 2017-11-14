#include <iostream>
#include <SFML/Graphics.hpp>
#include <slider/Slider.h>

int
main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "Slider");
    sf::CircleShape circle(100.f);
    circle.setFillColor(sf::Color::Magenta);
    Slider slider(30);


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