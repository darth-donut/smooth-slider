//
// @author: jiahong
// @date  : 14/11/17 10:41 PM
//

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <unordered_set>
#include <cassert>
#include <random>

#include "Slider.h"
#include "commons/util.h"

#define MAX_BLOCKS 3    // 0 - 3 blocks allowed



Board::Board(size_type size, bool blocks) : bsize(size) {
    // the board is never filled, need to let it know its bsize in advance
    board.resize(size);
    for (int i = 0; i != size; ++i) {
        board[i].resize(size);
    }

    // fill board now
    reset_board();

    // init player positions
    initialize_piece_positions();

    if (blocks) {
        randomize_blocks();
    }
}


Board::Board(const std::string &str_board) {
    auto rows = tokenize(str_board, '\n');
    bsize = rows.size();
    board.resize(bsize);

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
    assert(i == bsize);
}

void
Board::reset_board() {
    for (int i = 0; i != bsize; ++i) {
        std::fill(board[i].begin(), board[i].end(), SliderPiece::Blank);
        if (i != bsize - 1) {
            board[i][0] = SliderPiece::Horizontal;
        } else {
            for (int j = 1; j != bsize; ++j) {
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

bool
Board::make_move(const Move &move) {
    if (is_legal(move)) {
        // it's a legal move - remember to update our hashed set
        update_piece_positions(move);
        auto old_x = move.get_coord().first, old_y = move.get_coord().second;
        // if it wasn't an edge move, need to put a new piece
        // at the new coordinate, otherwise it's just out of the board
        if (!is_edge_move(move)) {
            auto new_coord = move.apply_move();
            board[new_coord.first][new_coord.second] = board[old_x][old_y];
        }
        board[old_x][old_y] = SliderPiece::Blank;
        return true;
    }
    return false;
}

bool
Board::is_legal(const Move &move) const {
    const auto &old_coord = move.get_coord();
    try {
        const auto &new_coord = move.apply_move();
        const auto &requested_move = move.get_move();

        // clearly, you can't move an unmovable piece - that's ridiculous
        if (board[old_coord.first][old_coord.second] == SliderPiece::Blank ||
            board[old_coord.first][old_coord.second] == SliderPiece::Block) {
            return false;
        }

        // Edge moves are always legal, if the player wasn't making an edge move, then they have to stick in the bounds.
        if (is_edge_move(move)) {
            return true;
        } else if (new_coord.first < 0 || new_coord.first >= bsize || new_coord.second < 0 || new_coord.second >= bsize) {
            // this is the glorious bounds check. Make sure it doesn't cross the board's bounds!
            return false;
        }

        // here, we know that we can safely index board (bounds check done above)

        // Can't move on a blocked space! (i.e. currently the game rules only allow you to move to
        // spaces that has blanks)
        if (board[new_coord.first][new_coord.second] != SliderPiece::Blank) {
            return false;
        }

        // Next, 1. make sure that the player is allowed to make the move
        //       2. check board's boundary conditions.
        //          There's a condition for a player's piece to "fall over the edge of the board"
        //          - Horizontal player reached the rightmost cell
        //                                 OR
        //          - Vertical player has reached the topmost cell
        switch (move.get_player()) {
            case SliderPlayer::Horizontal:
                if (board[old_coord.first][old_coord.second] != SliderPiece::Horizontal
                    || requested_move == SliderMove::Left) {
                    return false;
                }
                break;
            case SliderPlayer::Vertical:
                if (board[old_coord.first][old_coord.second] != SliderPiece::Vertical
                    || requested_move == SliderMove::Down) {
                    return false;
                }
                break;
        }

        // clear to go
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

void
Board::initialize_piece_positions() {
    for (auto i = 1, j = 0; i != bsize; ++i, ++j) {
        // bottom-most row of board
        vert_piece_positions.insert(std::make_pair(bsize - 1, i));
        // left-most column of board
        hori_piece_positions.insert(std::make_pair(j, 0));
    }
}

void
Board::update_piece_positions(const Move &move) {
    auto &hash_set = move.get_player() == SliderPlayer::Horizontal
                     ? hori_piece_positions
                     : vert_piece_positions;
    hash_set.erase(move.get_coord());
    if (!is_edge_move(move)) {
        // if it wasn't an edge move => the piece still remains on the board - put it(updated) back to the board
        hash_set.insert(move.apply_move());
    }
}

void
Board::randomize_blocks() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_type> block_dist(0, MAX_BLOCKS);
    std::uniform_int_distribution<size_type> x_gen(0, bsize - 2);
    std::uniform_int_distribution<size_type> y_gen(1, bsize - 1);
    size_t blocks = block_dist(mt);
    for (auto i = 0; i < blocks; ++i) {
        size_type x = x_gen(mt);
        size_type y = y_gen(mt);
        assert(board[x][y] == SliderPiece::Blank || board[x][y] == SliderPiece::Block);
        board[x][y] = SliderPiece::Block;
    }
}
