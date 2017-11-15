//
// @author: jiahong
// @date  : 14/11/17 10:11 PM
//

#ifndef SLIDER_MOVE_H
#define SLIDER_MOVE_H


enum class SliderPlayer : char;

enum class SliderMove : char {
    Up, Down, Left, Right
};

class Move {
public:
    /// Constructs a move consisting of a player (that was responsible for this move)
    /// and the directional move itself
    /// \param player Player responsible for this move
    /// \param move Direction of type SliderMove
    Move(const SliderPlayer player, SliderMove move)
            : player(player), move(move) {}

    /// Gets the player that was responsible for making this move
    /// \return player that was responsible for making this move of type SliderPlayer
    SliderPlayer get_player() const { return player; }

    /// Gets the directional move that some player made
    /// \return Directional move SliderMove
    SliderMove get_move() const { return move; }

private:
    SliderPlayer player;
    SliderMove move;
};


#endif //SLIDER_MOVE_H
