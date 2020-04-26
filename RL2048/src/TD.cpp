#include "../headers/TD.hpp"

void learn(Model *model, const Board &s1, const Board &s2) {
    auto actions = s2.possibleMoves();
    if (!actions.empty()) {
        double bestValue = 0;

        for (int action : actions) {
            auto[s2_1, r] = s2.move(action);
            bestValue = std::max(bestValue, r + model->apply(s2_1));
        }

        model->update(s1, bestValue);
    }
}

std::tuple<Board, int> playGame(Model *model) {
    int score = 0;
    Board s;

    while (true) {
        auto actions = s.possibleMoves();
        if (actions.empty()) {
            break;
        }

        int reward = 0;
        Board s1;

        double max_eval = std::numeric_limits<double>::lowest();
        for (int action : actions) {
            auto[b, r] = s.move(action);

            double eval = static_cast<double>(r) + model->apply(b);

            if (eval >= max_eval) {
                max_eval = eval;
                s1 = b;
                reward = r;
            }
        }

        Board s2(s1);
        s2.addRandom();

        learn(model, s1, s2);

        score += reward;
        s = s2;
    }

    return std::make_tuple(s, score);
}