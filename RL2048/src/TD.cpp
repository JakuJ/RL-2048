#include "../headers/TD.hpp"
#include "../headers/Ensemble.hpp"

void learn(Model *model, double s1Val, const Board &s1, const Board &s2) {
    auto actions = s2.possibleMoves();
    if (actions.empty()) {
        model->update(s1, -s1Val);
    } else {
        double bestValue = std::numeric_limits<double>::lowest();

        for (int action : actions) {
            auto[s3, r] = s2.move(action);
            bestValue = std::max(bestValue, r + model->apply(s3));
        }

        model->update(s1, bestValue - s1Val);
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
        double s1_val = 0;
        Board s1;

        double bestValue = std::numeric_limits<double>::lowest();
        for (int action : actions) {
            auto[b, r] = s.move(action);

            double utility = model->apply(b);
            double value = static_cast<double>(r) + utility;

            if (value >= bestValue) {
                bestValue = value;
                s1 = b;
                s1_val = utility;
                reward = r;
            }
        }

        Board s2(s1);
        s2.addRandom();

        learn(model, s1_val, s1, s2);

        Ensemble* ensemble = dynamic_cast<Ensemble*>(model);
		
		ensemble->copyLUTs(s1.stage);

        score += reward;
        s = s2;
    }

    return std::make_tuple(s, score);
}
