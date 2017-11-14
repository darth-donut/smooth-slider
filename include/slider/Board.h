//
// @author: jiahong
// @date  : 14/11/17 10:41 PM
//

#ifndef SLIDER_BOARD_H
#define SLIDER_BOARD_H

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "Move.h"

/*
 * (0,0) -----------------> (N,0)
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * |                        |
 * v                        v
 * (0, N) ----------------> (N,N)
 */

enum class SliderPiece {
    Vertical, Horizontal, Blank
};

class Board {
    friend std::ostream &operator<<(std::ostream &, const Board &);

public:
    typedef std::size_t size_type;

    explicit Board(const std::string &board);

    explicit Board(const std::size_t size);

    bool make_move(const Move &move);

    bool is_legal(const Move &move) const;

    size_type get_size() const { return size; }


private:
    std::vector<std::vector<SliderPiece>> board;

private:
    /// resets the board to the starting config
    void reset_board();

    std::size_t size;
};


#endif //SLIDER_BOARD_H
