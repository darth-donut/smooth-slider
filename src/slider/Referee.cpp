//
// @author: jiahong
// @date  : 26/11/17 6:57 PM
//

#include <cassert>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Referee.h"
#include "SliderGUI.h"

std::pair<SliderPlayer, bool>
Referee::start_game(bool disp_interm) {
    using std::swap;
    // current_player is the player that has to make the move this round
    // other_player is the 'other" player that made its move in the previous round
    auto tmp = get_players();
    auto current_player = tmp.first;
    auto other_player = tmp.second;

    while (!slider_board.has_winner() && !draw_game()) {
        Move pending_move;
        current_player->next_move(pending_move);
        // we can be 100% sure it's a legal move because next_move() of slider always checks for illegal moves.
        slider_board.make_move(pending_move);
        // inform other player that we've updated the board
        other_player->update(pending_move);
        swap(current_player, other_player);
        if (disp_interm) {
            std::cout << slider_board  << std::endl;
        }
    }

    return std::make_pair(slider_board.get_winner(), draw_game());
}

void
Referee::sanity_check() const {
    // check 1 - make sure that the players are enemies of each other
    if (p1->get_agent() == SliderPlayer::Horizontal) {
        assert(p2->get_agent() == SliderPlayer::Vertical);
    } else {
        assert(p2->get_agent() == SliderPlayer::Horizontal);
    }
    // check 2 - make sure that the 1st player is consistent
    assert(p1->get_player() == p2->get_player());
}

std::pair<std::shared_ptr<Slider>, std::shared_ptr<Slider>>
Referee::get_players() const {
    return p1->get_agent() == p1->get_player() ? std::make_pair(p1, p2) : std::make_pair(p2, p1);
}

bool
Referee::draw_game() const {
    return p1->possible_moves().empty() && p2->possible_moves().empty();
}

void
Referee::update() {
    if (window->isOpen()) {
        window->clear();
        using std::swap;

        if (!slider_board.has_winner() && !draw_game()) {
            Move pending_move;
            current_player->next_move(pending_move);
            if (current_player->ready_to_move()) {
                // we can be 100% sure it's a legal move because next_move() of slider always checks for illegal moves.
                slider_board.make_move(pending_move);
                // inform other player that we've updated the board
                other_player->update(pending_move);
                swap(current_player, other_player);
                // there maybe an error message saying "illegal move! from the prev frame"
                // tell window that we don't need that anymore
                window->display_error(false);
            } else {
                if (pending_move.is_bad_move()) {
                    // explicitly tell window to display error message associated with the bad move
                    window->err_msg(pending_move.get_err_msg());
                }  // else, it means user hasn't specified any kind of input
            }
        }
        draw_gui();
        window->disp();
    }
}

void
Referee::draw_gui() {
    constexpr float box_size = BOX_SIZE;
    const float x = window->getSize().x * WIDTH_PAD;
    const float y = window->getSize().y  * HEIGHT_PAD;
    for (size_t i = 0; i != slider_board.size(); ++i) {
        for (size_t j = 0; j != slider_board.size(); ++j) {
            sf::Vector2f pos(x + i * box_size, y + j * box_size);
            sf::RectangleShape box(sf::Vector2f(box_size, box_size));
            box.setPosition(pos);
            if ((j + i) % 2) {
                box.setFillColor(sf::Color::Green);
            } else {
                box.setFillColor(sf::Color::White);
            }
            window->draw(box);
            // draw board piece if there is one (incl blocks)
            sf::CircleShape piece(box_size / 3, 7);
            piece.setOrigin(box_size / 3, box_size / 3);
            switch (slider_board[j][i]) {
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
            if (slider_board[j][i] != SliderPiece::Blank) {
                sf::Vector2f piece_pos(pos.x + box_size / 2, pos.y + box_size / 2);
                piece.setPosition(piece_pos);
                window->draw(piece);
            }
        }
    }
}

