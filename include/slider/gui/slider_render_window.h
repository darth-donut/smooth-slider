//
// @author: jiahong
// @date  : 8/12/17 4:08 AM
//

#ifndef SLIDER_SLIDER_RENDER_WINDOW_H
#define SLIDER_SLIDER_RENDER_WINDOW_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <iostream>
#include <SFML/Graphics/Text.hpp>
#include <utility>
#include <SFML/Graphics/CircleShape.hpp>
#include "SliderGUI.h"

class SliderRenderWindow : public sf::RenderWindow {
public:
    // inherit ALL constructors from Renderwindow
    using sf::RenderWindow::RenderWindow;
    void err_msg(const std::string& msg) {
        err_text = msg;
        disp_error = true;
    }
    void display_error(bool toggle) { disp_error = toggle; }
    // "overrides display of RenderWindow" by also automatically showing error text if toggled
    void disp() {
        if (disp_error) {
            sf::Font font;
            font.loadFromFile("../res/fonts/AllerDisplay.ttf");
            sf::Text text;
            text.setFont(font);
            text.setPosition(getSize().x / 4, 10);
            text.setString(err_text);
            text.setColor(sf::Color::Red);
            text.setCharacterSize(ERR_TEXT_SIZE);
            //err_text.setStyle(sf::Text::Bold /* | sf::Text:Underlined*/);
            draw(text);
        }
        display();
    }
private:
    std::string err_text;
    bool disp_error = false;
};


#endif //SLIDER_SLIDER_RENDER_WINDOW_H
