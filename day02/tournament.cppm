module;

#include <vector>
#include <ranges>
#include <algorithm>
#include <numeric>

export module day02.tournament;

export namespace day02::tournament {

uint64_t follow_choice(const std::vector<std::pair<char,char>>& record) {
    auto score = [](auto& pair) -> uint64_t {
        // Transform both inputs numbers:
        // 0 == rock, 1 == paper, 2 == scissors
        int oponent = pair.first-'A';
        int me = pair.second-'X';

        // Score for the choice:
        // Rock == 1, Paper == 2, Scissors == 3
        int score = me + 1;

        // Score for the result:
        if (oponent == me) return score + 3; // draw
        // Winining choice has a rank one higher then the oposing choice
        // the only wrinkle is Rock beating Scissors which is why we need modulo.
        if ((me - oponent + 3) % 3 == 1) return score + 6;
        return score; // lose
    };
    // Finally, simply reduce to get the total sum of each match transformed into a score.
    auto scored = record | std::views::transform(score) | std::views::common;
    return std::reduce(scored.begin(), scored.end());
}

uint64_t follow_result(const std::vector<std::pair<char,char>>& record) {
    auto score = [](auto& pair) -> uint64_t {
        // Transform both inputs numbers:
        // 0 == rock, 1 == paper, 2 == scissors
        int oponent = pair.first-'A';
        int me = pair.second-'X';

        // Score for the result:
        // X (lose) == 0, Y (draw) == 3, Z (win) == 6
        int score = me*3;

        // On draw, we chose the same thing as the oponent
        if (me == 1) return score + oponent + 1; // draw
        // On a win, our choise was one rank higher than the oponnet,
        // again with the Rock/Scissors caveat requiring a modulo.
        if (me == 2) return score + (oponent+1)%3 + 1; // win
        // On a loss, our choice was one rak lower than the oponent,
        // again with the Scissors/Rock caveat, requiring a modulo.
        return score + (oponent-1+3)%3 + 1; // lose
    };
    // Finally, simply reduce to get the total sum of each match transformed into a score.
    auto scored = record | std::views::transform(score) | std::views::common;
    return std::reduce(scored.begin(), scored.end());
}

}