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
#include <queue>
#include <ranges>
#include <map>

export module day18.obsidian;

export namespace day18::obsidian {
struct Cube {
    int32_t x;
    int32_t y;
    int32_t z;
    // Generate all 6 neighbours
    std::vector<Cube> neighbours() const {
        return {{x-1,y,z},{x+1,y,z},{x,y-1,z},{x,y+1,z},{x,y,z-1},{x,y,z+1}};
    }
    friend std::istream& operator>>(std::istream& s, Cube& cube) {
        char delim;
        return s >> cube.x >> delim >> cube.y >> delim >> cube.z;
    }
};

int32_t count_uncovered_sides(const std::vector<Cube>& cubes) {
    std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> db;
    int total_sides = 0;
    // Pre-process all cubes
    for (auto &cube : cubes)
        db[cube.x][cube.y].insert(cube.z);

    for (auto &cube : cubes) {
        // Start with 6 sides uncovered.
        int sides = 6;
        // Examine all neighbours
        for (auto &n : cube.neighbours())
            // if any of them are cubes
            if (db[n.x][n.y].contains(n.z))
                // decrease the uncovered sides count accordingly
                --sides;
        total_sides += sides;
    }
    return total_sides;
}

int32_t external_surface(const std::vector<Cube>& cubes) {
    std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> db;
    int total_sides = 0;
    Cube boundary{-1,-1,-1};

    // While pre-processing, also remember the outermost coordinates
    auto update_boundary = [&boundary](const Cube& cube) {
        if (cube.x > boundary.x) boundary.x = cube.x;
        if (cube.y > boundary.y) boundary.y = cube.y;
        if (cube.z > boundary.z) boundary.z = cube.z;
    };
    for (auto &cube : cubes) {
        db[cube.x][cube.y].insert(cube.z);
        update_boundary(cube);
    }

    // BFS search
    std::queue<Cube> q;
    
    // Start from what is definitely the outside.
    // We could also start at {boundary.x+1,boundary.y+1,boundary.z+1}
    q.push(Cube{-1,-1,-1});

    // Remember visited cubes
    std::unordered_map<int, std::unordered_map<int, std::unordered_set<int>>> visited;
    visited[-1][-1].insert(-1);

    while (!q.empty()) {
        Cube curr = q.front();
        q.pop();

        for (auto &n : curr.neighbours()) {
            // Do not search more than one beyond boundary.
            if (n.x < -1 || n.x > boundary.x+1 ||
                n.y < -1 || n.y > boundary.y+1 ||
                n.z < -1 || n.z > boundary.z+1)
            continue;
            // Already visited
            if (visited[n.x][n.y].contains(n.z))
                continue;
            // The outside touching a cube, count it.
            if (db[n.x][n.y].contains(n.z)) {
                total_sides++;
                continue;
            }
            // This space was not yet visited and is not a cube
            visited[n.x][n.y].insert(n.z);
            q.push(n);
        }
    }

    return total_sides;
}

}