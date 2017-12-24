//
// @author: jiahong
// @date  : 10/12/17 4:17 AM
//

#ifndef SLIDER_TRAINING_H
#define SLIDER_TRAINING_H


#include <cstdlib>
#include <mutex>
#include "slider/slider_utils.h"

class Trainer {
public:
    Trainer(std::size_t board_size, SliderPlayer starting_player, std::size_t ngames, std::size_t threads)
            : board_size(board_size),
              starting_player(starting_player),
              ngames(ngames),
              threads(threads) {}

    void begin_training();

private:
    void play_games(std::vector<Model> &model_vector);
    void manage_games(size_t ngames);

private:
    std::size_t board_size;
    SliderPlayer starting_player;
    std::size_t ngames;
    std::mutex model_mutex;
    std::mutex vector_lock;
    std::size_t threads;
};


#endif //SLIDER_TRAINING_H
