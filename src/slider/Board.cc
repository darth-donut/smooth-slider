//
// @author: jiahong
// @date  : 14/11/17 10:41 PM
//

#include "Board.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "commons/util.h"

Board::Board(const std::size_t size) : size(size) {
    // the board is never filled, need to let it know its size in advance
    board.resize(size);
    for (int i = 0; i != size; ++i) {
        board[i].resize(size);
    }

    // fill board now
    reset_board();
}


Board::Board(const std::string &str_board) {
    auto rows = tokenize(str_board, '\n');
    size = rows.size();
    board.resize(size);
    
    size_type i = 0;
    
    for (const auto &row : rows) {
        for (auto piece : row) {
            switch (piece) {
                case 'B':
                    board[i].push_back(SliderPiece::Block);
                    break;
                case 'H':
                    board[i].push_back(SliderPiece::Horizontal);
                    break;
                case ' ':
                    board[i].push_back(SliderPiece::Blank);
                    break;
                case 'V':
                    board[i].push_back(SliderPiece::Vertical);
                    break;
                default:
                    throw std::domain_error("Unknown piece encountered!");
            }
        }
        // next row, please.
        ++i;
    }

}

void
Board::reset_board() {
    for (int i = 0; i != size; ++i) {
        std::transform(board[i].begin(),
                       board[i].end(),
                       board[i].begin(),
                       [](SliderPiece _) { return SliderPiece::Blank; });
        if (i != size - 1) {
            board[i][0] = SliderPiece::Horizontal;
        } else {
            for (int j = 1; j != size; ++j) {
                board[i][j] = SliderPiece::Vertical;
            }
        }
    }
}

std::ostream &
operator<<(std::ostream &os, const Board &board) {
    std::string buffer;
    for (auto row : board.board) {
        for (auto col : row) {
            char c;
            switch (col) {
                case SliderPiece::Blank:
                    c = ' ';
                    break;
                case SliderPiece::Horizontal:
                    c = 'H';
                    break;
                case SliderPiece::Vertical:
                    c = 'V';
                    break;
                case SliderPiece::Block:
                    c = 'B';
            }
            buffer.push_back(c);
            buffer.push_back(' ');
        }
        buffer.pop_back();      // don't need the last space
        buffer.push_back('\n');
    }
    buffer.pop_back();      // don't need the last newline
    return os << buffer;
}

