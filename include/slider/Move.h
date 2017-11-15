//
// @author: jiahong
// @date  : 14/11/17 10:11 PM
//

#ifndef SLIDER_MOVE_H
#define SLIDER_MOVE_H

#include <limits>
#include <stdexcept>

enum class SliderPlayer : char;
enum class SliderPiece : char;

enum class SliderMove : char {
    Up, Down, Left, Right
};

/// Immutable class Move
class Move {
public:
    typedef std::vector<SliderPiece>::size_type size_type;
    using Coordinate = std::pair<size_type, size_type>;

    /// Constructs a move consisting of a player (that was responsible for this move)
    /// and the directional move itself from specified coordinate
    /// \param player Player responsible for this move
    /// \param move Direction of type SliderMove
    /// \param coord Current coordinate
    Move(const SliderPlayer player, SliderMove move, const Coordinate &coord)
            : player(player), move(move), coord(coord) {}

    /// Gets the player that was responsible for making this move
    /// \return player that was responsible for making this move of type SliderPlayer
    SliderPlayer get_player() const { return player; }

    /// Gets the directional move that some player made
    /// \return Directional move SliderMove
    SliderMove get_move() const { return move; }

    /// Gets the coordinate of this move (starting coordinate)
    /// The final destination of this move is coord + get_move()
    /// \return coordinate of current initial move
    const Coordinate &get_coord() const { return coord; }

    /// Applies the supplied move to current coordinate and returns the new coordinate
    /// It's up to the user to check that the values are in bounds of the board / game. However, it does
    /// check and make sure that the bounds are not violated. The moves will not reach negative coordinates,
    /// nor will it overflow the specified size_type limits. Check this using std::numeric_limits<size_type>::max().
    /// In the event of overflow or negative values, exceptions will be thrown
    /// \return New Coordinate after move is applied to original coordinate. Under/overflow exceptions otherwise
    inline Coordinate apply_move() const;

private:
    SliderPlayer player;
    SliderMove move;
    Coordinate coord{};
};

Move::Coordinate
Move::apply_move() const {
    short x = 0, y = 0;
    switch (move) {
        case SliderMove::Up:
            x = -1;
            break;
        case SliderMove::Down:
            x = 1;
            break;
        case SliderMove::Right:
            y = 1;
            break;
        case SliderMove::Left:
            y = -1;
            break;
    }

    constexpr auto max_lim = std::numeric_limits<size_type>::max();
    if ((coord.first == 0 && x == -1)
        || (coord.second == 0 && y == -1)) {
        throw std::underflow_error("Move is underflowing from minimum allowed unsigned integer value");
    } else if ((coord.first == max_lim && x == 1)
        || (coord.second == max_lim) && y == 1) {
        throw std::overflow_error("Move is overflowing from maximum allowed unsigned integer value");
    }
    return std::make_pair(coord.first + x, coord.second + y);
}


#endif //SLIDER_MOVE_H
