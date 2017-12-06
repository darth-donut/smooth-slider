//
// @author: jiahong
// @date  : 7/12/17 3:53 AM
//

#include <SFML/Window/Event.hpp>
#include "slider/gui/SliderGUI.h"
#include "Board.h"

SliderGUI::SliderGUI(SliderPlayer agent, size_t size, SliderPlayer player, const sf::RenderWindow& window) :
    Slider(agent, size, player, nullptr),
    window(window) { }

Move
SliderGUI::next_move() {
    // not ready to move just yet (still waiting for user input)
    ready = false;
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    int x, y;
    double width_padding = window.getSize().x * 3.0 / 10;
    double height_padding = window.getSize().y * 1.0 / 10;
    x = static_cast<int>((mouse.x - width_padding) / 50);
    y = static_cast<int>((mouse.y - height_padding) / 50);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!accept_mouse_response) {
            mouse_clk_pos = Move::Coordinate(x, y);
//            std::cout << "down: " << x << ", " << y << std::endl;
            accept_mouse_response = true;
        }
    } else {
//        std::cout << "up: " << x << ", " << y << std::endl;
        if (accept_mouse_response) {
            accept_mouse_response = false;
            try {
                Move move(agent, board_to_vector(mouse_clk_pos), board_to_vector(Move::Coordinate(x, y)), board.size());
                if (board.is_legal(move)) {
                    ready = true;
                    std::cout << "Move accepted: " << std::endl;
                    update(move);
                    return move;
                } else {
                    std::cout << "Move illegal!" << std::endl;
                }
            } catch (const std::exception& e) {
                // todo
                std::cout << "Move illegal!: " << e.what() << std::endl;
            }
        }
    }
}


