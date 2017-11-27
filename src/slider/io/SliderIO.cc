//
// @author: jiahong
// @date  : 26/11/17 8:33 PM
//

#include <iostream>
#include <sstream>
#include <string>

#include "slider/io/SliderIO.h"
#include "Move.h"

#define ALPHA_START 65      // A
#define NUMERIC_START 48    // 0

Move
SliderIO::next_move() {
    do {
        // print board and flush
        std::cout << decorated_board() << '\n' << "You are "
                  << (player == SliderPlayer::Horizontal ? "Horizontal" : "Vertical")
                  << ", your move: ";
        // read input from user
        std::string input;
        std::cin >> input;
        try {
            Move move(player, input, board.size());
            if (board.is_legal(move)) {
                // update our own board before returning valid move
                update(move);
                return move;
            } else {
                // print and immediately flush
                std::cout << "Move requested isn't legal, try again!" << std::endl;
            }
        } catch (const std::exception &e) {
            // print and immediately flush
            std::cout << e.what() << " , try again!" << std::endl;
        }
    } while (true);
}

std::string
SliderIO::decorated_board() const {

    std::ostringstream os;
    os << board;

    std::string str_board = os.str();

    /* first, add the alpha col:
     * A _ _ _ _ _ _
     * B _ _ _ _ _ _
     * C _ _ _ _ _ _
     * D _ _ _ _ _ _
     * E _ _ _ _ _ _
     * */
    Board::size_type alpha_offset = 0;
    str_board.insert(0, std::string(1, static_cast<char>(ALPHA_START + (alpha_offset++))) + ' ');

    // while it's not the last newline and there's more newlines - add an alphabet right after it
    // observe, E is the last alphabet to be inserted, right before the last newline (and the last newline exists at
    // pos = str_board.bsize() - 1)
    for (auto pos = str_board.find('\n'); pos != str_board.size() - 1 && pos != std::string::npos; ++alpha_offset) {
        str_board.insert(pos + 1, std::string(1, static_cast<char>(ALPHA_START + alpha_offset)) + ' ');
        pos = str_board.find('\n', pos + 1);
    }

    /* now, add the numeric row:
     * A _ _ _ _ _ _
     * B _ _ _ _ _ _
     * C _ _ _ _ _ _
     * D _ _ _ _ _ _
     * E _ _ _ _ _ _
     *   0 1 2 3 4 5
     */

    str_board.append("\n  ");
    for (auto i = NUMERIC_START; i < NUMERIC_START + board.size(); ++i) {
        str_board.push_back(static_cast<char>(i));
        str_board.push_back(' ');
    }
    str_board.pop_back();
    return str_board;
}
