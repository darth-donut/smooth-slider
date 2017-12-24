//
// @author: jiahong
// @date  : 7/12/17 3:53 AM
//

#include <SFML/Window/Event.hpp>
#include<cassert>

#include "agents/gui/SliderGUI.h"
#include "Board.h"
#include "util.h"

SliderGUI::SliderGUI(SliderPlayer agent, const Board &board, SliderPlayer player, const sf::RenderWindow& window) :
    Slider(agent, board, player, nullptr, nullptr),
    window(window) { }

void
SliderGUI::next_move(Move &p_move) {
    // not ready to move just yet (still waiting for user input)
    ready = false;
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    double width_padding = window.getSize().x * WIDTH_PAD;
    double height_padding = window.getSize().y * HEIGHT_PAD;

    // x and y will be the coordinate of mouse position translated to board representation index
    // see Board.h for information
    Board::size_type x, y;
    x = static_cast<Board::size_type >((mouse.x - width_padding) / BOX_SIZE);
    y = static_cast<Board::size_type >((mouse.y - height_padding) / BOX_SIZE);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!accept_mouse_response && player_piece_clicked(x, y)) {
            mouse_clk_pos = Move::Coordinate(x, y);
            accept_mouse_response = true;
        }
    } else {
        if (accept_mouse_response) {
            accept_mouse_response = false;
            try {
                Move move(agent, board_to_vector(mouse_clk_pos), board_to_vector(Move::Coordinate(x, y)), board.size());
                if (board.is_legal(move)) {
                    ready = true;
//                    std::cout << "Move accepted: " << move << std::endl;
                    update(move);
                    p_move = move;
                    return;
                } else {
                    Move m;
                    m.error("Illegal move, Try again!");
                    p_move = m;
                    return;
                }
            } catch (const std::exception& e) {
                Move m;
                m.error("Illegal move: " + std::string(e.what()) + "!");
                p_move = m;
                return;
            }
        }
    }
    // return default move if user doesn't supply any input - this is a safe operation because
    // ready is still set to false unless user supplied a legal move & we made sure to call
    // is_ready() in Referee.cpp
    assert(!ready);
    p_move = Move();
}

bool
SliderGUI::player_piece_clicked(Board::size_type x, Board::size_type y) const {
    auto translated = board_to_vector(Move::Coordinate(x,  y));

    // before indexing vector - check that it's in bounds (clicks outside of the board are also registered)
    if (translated.first < 0 || translated.first >= board.size() ||
            translated.second < 0 || translated.second >= board.size()) {
        return false;
    }
    auto player_piece = (agent == SliderPlayer::Horizontal)
                        ? SliderPiece::Horizontal
                        : SliderPiece::Vertical;
    return board[translated.first][translated.second] == player_piece;
}

