module;

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <variant>
#include <numeric>
#include <compare>
#include <sstream>
#include <ranges>
#include <map>
#include <set>
#include <list>
#include <queue>

export module day24.blizzards;

export namespace day24::blizzards {

// One row of blizzards
struct BlizzardRow {
    std::vector<bool> left_going;
    std::vector<bool> right_going;
    bool is_empty(int64_t column, int64_t time) const {
        int64_t cnt = std::ssize(left_going);
        // determine which blizzard will be at this index at a given time
        // (-1) offset since blizzards only move in the inner area (excluding map border)
        int64_t left_idx = (column - 1 + time) % cnt;
        int64_t right_idx = (column - 1 + cnt - (time % cnt)) % cnt;
        return !(left_going[left_idx] || right_going[right_idx]);
    }
};

// One column of blizzards
struct BlizzardCol {
    std::vector<bool> up_going;
    std::vector<bool> down_going;
    bool is_empty(int64_t row, int64_t time) const {
        int64_t cnt = std::ssize(up_going);
        // determine which blizzard will be at this index at a given time
        // (-1) offset since blizzards only move in the inner area (excluding map border)
        int64_t up_idx = (row - 1 + time) % cnt;
        int64_t down_idx = (row - 1 + cnt - (time % cnt)) % cnt;
        return !(up_going[up_idx] || down_going[down_idx]);
    }
};

struct Map {
    std::vector<std::string> map;
    std::vector<BlizzardRow> blizzard_rows;
    std::vector<BlizzardCol> blizzard_cols;

    bool can_step(int64_t ridx, int64_t cidx, int64_t time) const {
        // always can step on start and end tiles
        if (ridx == 0 && cidx == 1)
            return true;
        if (ridx == std::ssize(map)-1 && cidx == std::ssize(map[0])-2)
            return true;

        // Out of bounds check
        if (ridx < 1 || ridx >= std::ssize(map)-1)
            return false;
        if (cidx < 1 || cidx >= std::ssize(map[0])-1)
            return false;

        // If we are within bounds, we can step if the blizzards are not on this tile
        // (-1) offset again since we store blizzards only for the inner area
        return blizzard_rows[ridx-1].is_empty(cidx, time) && 
            blizzard_cols[cidx-1].is_empty(ridx, time);
    }

    friend std::istream& operator>>(std::istream& s, Map& m) {
        std::string line;
        while (getline(s, line))
            m.map.push_back(line);

        for (int64_t cidx : std::views::iota(1z, std::ssize(m.map[0]) - 1))
            m.blizzard_cols.push_back(BlizzardCol{});

        for (int64_t ridx : std::views::iota(1z, std::ssize(m.map) - 1)) {
            m.blizzard_rows.push_back(BlizzardRow{});
            for (int64_t cidx : std::views::iota(1z, std::ssize(m.map[ridx]) - 1)) {
                if (m.map[ridx][cidx] == '.') {
                    m.blizzard_rows[ridx-1].left_going.push_back(false);
                    m.blizzard_rows[ridx-1].right_going.push_back(false);
                    m.blizzard_cols[cidx-1].up_going.push_back(false);
                    m.blizzard_cols[cidx-1].down_going.push_back(false);
                }
                if (m.map[ridx][cidx] == '>') {
                    m.blizzard_rows[ridx-1].left_going.push_back(false);
                    m.blizzard_rows[ridx-1].right_going.push_back(true);
                    m.blizzard_cols[cidx-1].up_going.push_back(false);
                    m.blizzard_cols[cidx-1].down_going.push_back(false);
                }
                if (m.map[ridx][cidx] == '<') {
                    m.blizzard_rows[ridx-1].left_going.push_back(true);
                    m.blizzard_rows[ridx-1].right_going.push_back(false);
                    m.blizzard_cols[cidx-1].up_going.push_back(false);
                    m.blizzard_cols[cidx-1].down_going.push_back(false);
                }
                if (m.map[ridx][cidx] == 'v') {
                    m.blizzard_rows[ridx-1].left_going.push_back(false);
                    m.blizzard_rows[ridx-1].right_going.push_back(false);
                    m.blizzard_cols[cidx-1].up_going.push_back(false);
                    m.blizzard_cols[cidx-1].down_going.push_back(true);
                }
                if (m.map[ridx][cidx] == '^') {
                    m.blizzard_rows[ridx-1].left_going.push_back(false);
                    m.blizzard_rows[ridx-1].right_going.push_back(false);
                    m.blizzard_cols[cidx-1].up_going.push_back(true);
                    m.blizzard_cols[cidx-1].down_going.push_back(false);
                }
                m.map[ridx][cidx] = '.';
            }
        }
        return s;
    }
};

struct Pos {
    size_t row;
    size_t col;
};

int64_t search(const Map& m, Pos start, Pos end, int64_t start_time = 0) {
    // We could do a complicated search by position, 
    // but it's a lot simpler to just consider 
    // all positions reachable at each time point.
    int64_t time = start_time;
    auto current = m.map;
    current[start.row][start.col] = 'X'; // at time == 0, only the start is reachable

    while (current[end.row][end.col] != 'X') { // until we reach the end
        auto next = m.map; // new empty map

        for (int64_t ridx : std::views::iota(0z, std::ssize(m.map))) {
            for (int64_t cidx : std::views::iota(0z, std::ssize(m.map[ridx]))) {
                // For any space reached
                if (current[ridx][cidx] == 'X') {
                    // Mark positions in next that are reachable in time+1
                    if (m.can_step(ridx, cidx, time+1))
                        next[ridx][cidx] = 'X';
                    if (m.can_step(ridx-1, cidx, time+1))
                        next[ridx-1][cidx] = 'X';
                    if (m.can_step(ridx+1, cidx, time+1))
                        next[ridx+1][cidx] = 'X';
                    if (m.can_step(ridx, cidx-1, time+1))
                        next[ridx][cidx-1] = 'X';
                    if (m.can_step(ridx, cidx+1, time+1))
                        next[ridx][cidx+1] = 'X';
                }
            }
        }
        // Advance time and flip the map
        ++time;
        current = std::move(next);
    }
    return time;
}


int64_t fetch_snack_search(const Map& m) {
    Pos start = {0,1};
    Pos end = {m.map.size()-1, m.map.at(0).size()-2};

    // Reach end
    int64_t time = search(m, start, end, 0);
    // Go back for snack
    int64_t back = search(m, end, start, time);
    // Reach end again
    int64_t result = search(m, start, end, back);

    return result;
}

}