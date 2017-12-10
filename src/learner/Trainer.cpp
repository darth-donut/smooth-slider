//
// @author: jiahong
// @date  : 10/12/17 4:17 AM
//

#include <slider/Slider.h>
#include <slider/Referee.h>
#include <slider/model.h>
#include <thread>
#include <slider/td_leaf_lambda.h>
#include "learner/Trainer.h"


void
Trainer::begin_training() {
    std::thread games[ngames];
    for (auto &game : games) {
        game = std::thread(&Trainer::play_games, this);
    }
    for (auto &game : games) {
        game.join();
    }
}

void
Trainer::play_games() {
    Minimax<Move, Slider> ai_strategy1{4};
    Minimax<Move, Slider> ai_strategy2{5};

    // file maybe updated, critical section
    model_mutex.lock();
    Model bob(Resource::bob_model);
    model_mutex.unlock();
    // end of critical section

    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board_size, starting_player, &ai_strategy1, &bob),
            std::make_shared<Slider>(SliderPlayer::Horizontal, board_size, starting_player, &ai_strategy2, &bob),
            board_size);
    auto winner = referee.start_game();
    if (!winner.second) {       // if it wasn't a draw
        std::cout << (winner.first == SliderPlayer::Horizontal ? "Horizontal" : "Vertical") << " won!\n";
        TDLeafLambda td_trainer(bob, model_mutex, referee.get_p1_stats());
        td_trainer.update_weights();
    } else {
        std::cout << "The game ended in a draw!\n";
    }
}
