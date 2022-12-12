module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <queue>

export module day12.hill;

export namespace day12::hill {
struct Pos {
    int64_t row;
    int64_t column;
    Pos left() { return {row,column-1}; }
    Pos right() { return {row,column+1}; }
    Pos down() { return {row+1,column}; }
    Pos up() { return {row-1,column}; }
};

int64_t hill_climb(const std::vector<std::string>& map, Pos start, Pos end) {
    std::vector<std::vector<int64_t>> visited(map.size(), std::vector<int64_t>(map[0].length(), INT64_MAX));
    
    auto elevation_ok = [](char from, char to) {
        if (from >= to) return true;
        return from+1 == to;
    };

    auto can_step = [&](Pos from, Pos to, int64_t cost) {
        // Check bounds
        if (to.row >= std::ssize(map) || to.row < 0) return false;
        if (to.column >= std::ssize(map[0]) || to.column < 0) return false;
        // Check if not longer than the shortest known path.
        if (cost + 1 >= visited[to.row][to.column]) return false;
        // Check whether the elevation change is ok.
        if (map[from.row][from.column] == 'S')
            return elevation_ok('a', map[to.row][to.column]);
        if (map[to.row][to.column] == 'E')
            return elevation_ok(map[from.row][from.column], 'z');
        return elevation_ok(map[from.row][from.column], map[to.row][to.column]);
    };

    std::queue<std::pair<Pos,int64_t>> queue;
    queue.push(std::make_pair(start,0));
    visited[start.row][start.column] = 0;

    while (!queue.empty()) {
        auto [from, cost] = queue.front();
        queue.pop();

        if (map[from.row][from.column] == 'E') continue;

        for (auto dir : {&Pos::left, &Pos::right, &Pos::up, &Pos::down}) {
            Pos to = (from.*dir)();
            if (!can_step(from, to, cost + 1)) continue;

            queue.push(std::make_pair(to, cost + 1));
            visited[to.row][to.column] = cost + 1;
        }
    }
    return visited[end.row][end.column];
}

int64_t hill_plan(const std::vector<std::string>& map, Pos end) {
    std::vector<std::vector<int64_t>> visited(map.size(), std::vector<int64_t>(map[0].length(), INT64_MAX));
    
    auto elevation_ok = [](char from, char to) {
        if (from >= to) return true;
        return from+1 == to;
    };

    // Check whether step is valid, but backwards.
    auto can_step = [&](Pos from, Pos to, int64_t cost) {
        // Check bounds
        if (to.row >= std::ssize(map) || to.row < 0) return false;
        if (to.column >= std::ssize(map[0]) || to.column < 0) return false;
        // Check if not longer than the shortest known path.
        if (cost + 1 >= visited[to.row][to.column]) return false;
        // Check whether the elevation change is ok.
        if (map[from.row][from.column] == 'E')
            return elevation_ok(map[to.row][to.column], 'z');
        if (map[to.row][to.column] == 'S')
            return elevation_ok('a', map[from.row][from.column]);
        return elevation_ok(map[to.row][to.column], map[from.row][from.column]);
    };

    std::queue<std::pair<Pos,int64_t>> queue;
    queue.push(std::make_pair(end,0));
    visited[end.row][end.column] = 0;
    int64_t best = INT64_MAX;

    while (!queue.empty()) {
        auto [from, cost] = queue.front();
        queue.pop();
        if (map[from.row][from.column] == 'S' || 
            map[from.row][from.column] == 'a') {
            // Update the best seen path from any 'a' or 'S' to 'E'.
            if (best > cost) best = cost;
            continue;
        }

        // Iterate over the directions
        for (auto dir : {&Pos::left, &Pos::right, &Pos::up, &Pos::down}) {
            Pos to = (from.*dir)();
            if (!can_step(from, to, cost + 1)) continue;

            queue.push(std::make_pair(to, cost + 1));
            visited[to.row][to.column] = cost + 1;
        }
    }
    return best;
}

}
