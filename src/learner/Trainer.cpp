//
// @author: jiahong
// @date  : 10/12/17 4:17 AM
//

#include <slider/Slider.h>
#include <slider/Referee.h>
#include <slider/model.h>
#include <thread>

#include "slider/td_leaf_lambda.h"
#include "agents/basic/forward_slider.h"
#include "learner/Trainer.h"


void
Trainer::begin_training() {
    auto rounds = ngames / threads;
    auto extras = ngames % threads;
    for (size_t i = 0; i < rounds; ++i) {
        manage_games(threads);
    }
    if (extras > 0) {
        manage_games(extras);
    }
}

void
Trainer::play_games(std::vector<Model> &model_vector) {
    Minimax<Move, Slider> ai_strategy1{7};

    // file maybe updated, critical section
    model_mutex.lock();
    Model bob(Resource::bob_model);
    model_mutex.unlock();
    // end of critical section

    // board design
    Board board(board_size);

    Referee referee(
            std::make_shared<Slider>(SliderPlayer::Vertical, board, starting_player, &ai_strategy1, &bob),
            std::make_shared<ForwardSlider>(SliderPlayer::Horizontal, board, starting_player),
            board);
    auto winner = referee.start_game();
    if (!winner.second) {       // if it wasn't a draw
        std::cout << (winner.first == SliderPlayer::Horizontal ? "Horizontal" : "Vertical") << " won!" << std::endl;
        TDLeafLambda td_trainer(bob, referee.get_p1_stats());
        vector_lock.lock();
        model_vector.push_back(td_trainer.update_weights());
        vector_lock.unlock();

    } else {
        std::cout << "The game ended in a draw!" << std::endl;
    }
}

void
Trainer::manage_games(size_t n) {
    std::vector<std::thread> games;
    std::vector<Model> updated_models;

    for (auto i = 0; i < n; ++i) {
        games.emplace_back(&Trainer::play_games, this, std::ref(updated_models));
    }

    for (auto &game : games) {
        game.join();
    }

    auto first_model = updated_models.front();
    Model::size_type nfeatures = first_model.size();
    Model avg_model(first_model);
    for (auto k = 0; k < nfeatures; ++k) {
        double kth_sum = 0;
        for (auto &updated_model : updated_models) {
            kth_sum += updated_model[k];
        }
        avg_model[k] = kth_sum / updated_models.size();
    }
    std::lock_guard<std::mutex> flush_guard(model_mutex);
    avg_model.flush();
}
