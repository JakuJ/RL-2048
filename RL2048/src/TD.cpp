#include "../headers/TD.hpp"
#include "../headers/Ensemble.hpp"

static void learn(Model *model, double s1Val, const Board &s1, const Board &s2) {
    const auto actions = s2.possibleMoves();
    if (std::none_of(actions.cbegin(), actions.cend(), [](auto x) { return x; })) {
        model->update(s1, -s1Val);
    } else {
        double bestValue = std::numeric_limits<double>::lowest();

        for (int i = 0; i < actions.size(); ++i) {
            if (actions[i]) {
                const auto&&[s3, r] = s2.swipe(i);
                bestValue = std::max(bestValue, r + model->apply(s3));
            }
        }

        model->update(s1, bestValue - s1Val);
    }
}

std::tuple<Board, int> playGame(Model *model) {
    int score = 0;

    Board s;
    s.addRandom();
    s.addRandom();

    for (;;) {
        const auto actions = s.possibleMoves();
        if (std::none_of(actions.cbegin(), actions.cend(), [](auto x) { return x; })) {
            break;
        }

        int reward = 0;
        double s1_val = 0;
        Board s1;

        double bestValue = std::numeric_limits<double>::lowest();
        for (int i = 0; i < actions.size(); ++i) {
            if (actions[i]) {
                const auto&&[b, r] = s.swipe(i);

                const double utility = model->apply(b);
                const double value = static_cast<double>(r) + utility;

                if (value >= bestValue) {
                    bestValue = value;
                    s1 = b;
                    s1_val = utility;
                    reward = r;
                }
            }
        }

        s = s1;  // s is s2
        s.addRandom();

        learn(model, s1_val, s1, s);

        score += reward;
    }

    return std::make_tuple(s, score);
}
