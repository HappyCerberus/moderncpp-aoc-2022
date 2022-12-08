module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>

export module day08.trees;

export namespace day08::trees {
size_t count_visible(const std::vector<std::string>& map) {
    size_t cnt = 0;
    char max = '0'-1;
    auto map_copy = map;

    // Counting lambda, if the current tree is higher 
    // than the tallest tree seen so far, then it will
    // be visible from the border.
    auto count = [&](size_t ridx, size_t cidx) {
        // Is the tree hidden?
        if (map[ridx][cidx] <= max) return;
        // Was this tree already counted?
        if (map_copy[ridx][cidx] != 'X')
            cnt++;
        // This is the new tallest tree.
        max = map[ridx][cidx];
        // Mark as counted.
        map_copy[ridx][cidx] = 'X';
    };
    
    // left-to-right and right-to-left
    for (auto ridx : std::views::iota(0uz, map.size())) {
        max = '0'-1; // reset the tallest seen tree
        for (auto cidx : std::views::iota(0uz, map[ridx].length()))
            count(ridx, cidx);
        max = '0'-1; // reset the tallest seen tree
        for (auto cidx : std::views::iota(0uz, map[ridx].length()) | std::views::reverse)
            count(ridx, cidx);
    }

    // top-down and bottom-to-top
    for (auto cidx : std::views::iota(0uz, map[0].length())) {
        max = '0'-1; // reset the tallest seen tree
        for (auto ridx : std::views::iota(0uz, map.size()))
            count(ridx, cidx);
        max = '0'-1; // reset the tallest seen tree
        for (auto ridx : std::views::iota(0uz, map.size()) | std::views::reverse)
            count(ridx, cidx);
    }
    return cnt;
}

size_t best_scenic_score(const std::vector<std::string>& map) {
    std::vector<std::vector<size_t>> scores(map.size(), std::vector<size_t>(map[0].length(), 1));
    
    // When iterating, skip over the border trees since all border trees have zero scenic score.
    // On top of that, the boundary trees can be considered to be all '9', since they effectively block the view (no view outside of the map).

    // left-to-right and right-to-left
    for (auto ridx : std::views::iota(1uz, map.size()-1)) {
        // last index where a tree of given size was seen
        std::vector<size_t> block(10, 0); 
        for (auto cidx : std::views::iota(1uz, map[ridx].length()-1)) {
            char c = map[ridx][cidx];
            // The score in this direction is the distance from a tree that would block the view.
            scores[ridx][cidx] *= cidx-block[c-'0'];
            // This tree blocks any tree of same size or smaller.
            for (auto val : std::views::iota(0, c-'0'+1))
                block[val] = cidx;
        }

        block = std::vector<size_t>(10, map[ridx].length()-1); // reset
        for (auto cidx : std::views::iota(1uz, map[ridx].length()-1) | std::views::reverse) {
            char c = map[ridx][cidx];
            // Same score calculation, but in the oposite direction.
            scores[ridx][cidx] *= block[c-'0']-cidx;
            for (auto val : std::views::iota(0, c-'0'+1))
                block[val] = cidx;
        }
    }

    size_t max = 0;
    // top-down and bottom-to-top
    for (auto cidx : std::views::iota(1uz, map[0].length()-1)) {
        std::vector<size_t> block(10, 0); // last index where a tree of given size was seen
        for (auto ridx : std::views::iota(1uz, map.size()-1)) {
            char c = map[ridx][cidx];
            // Same as before, but now we iterate over rows.
            scores[ridx][cidx] *= ridx-block[c-'0'];
            for (auto val : std::views::iota(0, c-'0'+1))
                block[val] = ridx;
        }

        block = std::vector<size_t>(10, map.size()-1); // reset
        for (auto ridx : std::views::iota(1uz, map.size()-1) | std::views::reverse) {
            char c = map[ridx][cidx];
            // Again rows, but in oposite direction.
            scores[ridx][cidx] *= block[c-'0']-ridx;
            // Might as well pick-up the maximum since 
            // we are already looping over the data.
            if (scores[ridx][cidx] > max)
                max = scores[ridx][cidx];
            for (auto val : std::views::iota(0, c-'0'+1))
                block[val] = ridx;
        }
    }
    return max;
}
}