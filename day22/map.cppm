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
#include <list>

export module day22.map;

export namespace day22::map {

struct Coord {
    int64_t row;
    int64_t col;
    friend auto operator<=>(const Coord&, const Coord&) = default;
};

struct Direction {
    int64_t row_diff;
    int64_t col_diff;
    Direction left() {
        if (col_diff != 0) return {-col_diff, 0};
        return {0, row_diff};
    }
    Direction right() {
        if (col_diff != 0) return {col_diff, 0};
        return {0, -row_diff};
    }
    Coord step(Coord coord) {
        return {coord.row + row_diff, coord.col + col_diff};
    }
    friend auto operator<=>(const Direction&, const Direction&) = default;
};

constexpr inline auto LEFT = Direction{0, -1};
constexpr inline auto RIGHT = Direction{0, 1};
constexpr inline auto UP = Direction{-1, 0};
constexpr inline auto DOWN = Direction{1, 0};

struct Map {
    std::vector<std::string> map;
    std::string directions;

    Coord step(Coord coord, Direction dir) {
        // Wrap-around logic
        auto left_border = [this](int64_t row) {
            for (int64_t idx : std::views::iota(0z, std::ssize(map[0])))
                if (map[row][idx] != ' ')
                    return idx;
            return 0z;
        };
        auto right_border = [this](int64_t row) {
            for (int64_t idx : std::views::iota(0z, std::ssize(map[0])) | std::views::reverse)
                if (map[row][idx] != ' ')
                    return idx;
            return std::ssize(map[0])-1;
        };
        auto top_border = [this](int64_t col) {
            for (int64_t idx : std::views::iota(0z, std::ssize(map)))
                if (map[idx][col] != ' ')
                    return idx;
            return 0z;
        };
        auto bottom_border = [this](int64_t col) {
            for (int64_t idx : std::views::iota(0z, std::ssize(map)) | std::views::reverse)
                if (map[idx][col] != ' ')
                    return idx;
            return std::ssize(map)-1;
        };

        Coord next = dir.step(coord);
        // stepping outside, wrap around
        if (next.row >= std::ssize(map) || next.row < 0 || 
            next.col >= std::ssize(map[0]) || next.col < 0 || 
            map[next.row][next.col] == ' ') {
            // Based on our direction, apply one of the wrap-around functions
            if (dir == UP)
                next = {bottom_border(coord.col), coord.col};
            if (dir == DOWN)
                next = {top_border(coord.col), coord.col};
            if (dir == LEFT)
                next = {coord.row, right_border(coord.row)};
            if (dir == RIGHT)
                next = {coord.row, left_border(coord.row)};
        }

        // If the space we are about step onto is a wall, don't move
        if (map[next.row][next.col] == '#')
            return coord;
        // Otherwise move
        return next;
    }

    int64_t follow_directions() {
        std::stringstream s(directions);

        // We start on the first empty tile on the first row
        Coord coord{0,0};
        for (int64_t idx : std::views::iota(0z, std::ssize(map[0])))
            if (map[0][idx] == '.') {
                coord = {0, idx};
                break;
            }
        // Oriented to the right
        Direction dir{0,1};

        while (s) {
            // number of steps
            if (std::isdigit(s.peek())) {
                int count;
                s >> count;
                // do the steps
                for (int64_t idx : std::views::iota(0, count))
                    coord = step(coord, dir);
            } else {
                // turn left or right
                char turn = s.get();
                if (!s) break;
                if (turn == 'L') dir = dir.left();
                if (turn == 'R') dir = dir.right();
            }
        }
        // Calculate the password
        int64_t result = 1000*(coord.row+1) + 4*(coord.col+1);
        if (dir == DOWN) result += 1;
        if (dir == LEFT) result += 2;
        if (dir == UP) result += 3;
        return result;
    }

    friend std::istream& operator>>(std::istream& s, Map& m) {
        std::string line;
        size_t max = 0;
        while (getline(s, line) && line != "") {
            m.map.push_back(std::move(line));
            if (m.map.back().length() > max)
                max = m.map.back().length();
        }
        std::ranges::for_each(m.map, [max](auto &line) {
            line.resize(max,' ');
        });
        return getline(s, m.directions);
    }
};




struct TiledPosition {
    int64_t tile;
    Coord coord;
    Direction dir;
};

struct TiledMap {
    int64_t TILE;
    std::string directions;
    std::vector<std::vector<std::string>> tiles;
    std::map<int64_t, std::map<Direction, std::function<TiledPosition(const TiledPosition&)>>> translation;
    TiledMap(const Map& m, const int64_t size = 50) : TILE(size), directions(m.directions) {
        std::vector<std::vector<int64_t>> tile_map{
            {-1,0,1},
            {-1,2,-1},
            {3,4,-1},
            {5,-1,-1}
        };

        tiles.resize(6);
        for (auto ridx : std::views::iota(0z, std::ssize(m.map))) {
            for (auto cidx : std::views::iota(0z, std::ssize(m.map[ridx]))) {
                int64_t tile_id = tile_map[ridx/TILE][cidx/TILE];
                if (tile_id != -1) {
                    tiles[tile_id].resize(TILE);
                    tiles[tile_id][ridx%TILE].resize(TILE);
                    tiles[tile_id][ridx%TILE][cidx%TILE] = m.map[ridx][cidx];
                }
            }
        }

        // For each tile and each direction, translate exiting coord 
        // into a new coord and direction on the target tile.
        translation[0][Direction{0,1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{1, Coord{pos.coord.row, 0}, Direction{0,1}};
        };
        translation[0][Direction{0,-1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{3, Coord{TILE-1-pos.coord.row, 0}, Direction{0,1}};
        };
        translation[0][Direction{1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{2, Coord{0, pos.coord.col}, Direction{1,0}};
        };
        translation[0][Direction{-1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{5, Coord{pos.coord.col, 0}, Direction{0,1}};
        };

        translation[1][Direction{0,1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{4, Coord{TILE-1-pos.coord.row, TILE-1}, Direction{0,-1}};
        };
        translation[1][Direction{0,-1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{0, Coord{pos.coord.row, TILE-1}, Direction{0,-1}};
        };
        translation[1][Direction{1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{2, Coord{pos.coord.col, TILE-1}, Direction{0,-1}};
        };
        translation[1][Direction{-1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{5, Coord{TILE-1, pos.coord.col}, Direction{-1,0}};
        };

        translation[2][Direction{0,1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{1, Coord{TILE-1, pos.coord.row}, Direction{-1,0}};
        };
        translation[2][Direction{0,-1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{3, Coord{0, pos.coord.row}, Direction{1,0}};
        };
        translation[2][Direction{1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{4, Coord{0, pos.coord.col}, Direction{1,0}};
        };
        translation[2][Direction{-1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{0, Coord{TILE-1, pos.coord.col}, Direction{-1,0}};
        };

        translation[3][Direction{0,1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{4, Coord{pos.coord.row, 0}, Direction{0,1}};
        };
        translation[3][Direction{0,-1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{0, Coord{TILE-1-pos.coord.row, 0}, Direction{0,1}};
        };
        translation[3][Direction{1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{5, Coord{0, pos.coord.col}, Direction{1,0}};
        };
        translation[3][Direction{-1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{2, Coord{pos.coord.col, 0}, Direction{0,1}};
        };

        translation[4][Direction{0,1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{1, Coord{TILE-1-pos.coord.row, TILE-1}, Direction{0,-1}};
        };
        translation[4][Direction{0,-1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{3, Coord{pos.coord.row, TILE-1}, Direction{0,-1}};
        };
        translation[4][Direction{1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{5, Coord{pos.coord.col, TILE-1}, Direction{0,-1}};
        };
        translation[4][Direction{-1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{2, Coord{TILE-1, pos.coord.col}, Direction{-1,0}};
        };

        translation[5][Direction{0,1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{4, Coord{TILE-1, pos.coord.row}, Direction{-1,0}};
        };
        translation[5][Direction{0,-1}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{0, Coord{0, pos.coord.row}, Direction{1,0}};
        };
        translation[5][Direction{1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{1, Coord{0, pos.coord.col}, Direction{1,0}};
        };
        translation[5][Direction{-1,0}] = [TILE=TILE](const TiledPosition& pos) {
            return TiledPosition{3, Coord{TILE-1, pos.coord.col}, Direction{-1,0}};
        };
    }

    TiledPosition step(const TiledPosition& coord) {
        TiledPosition next = coord;
        next.coord = next.dir.step(next.coord);

        // If we are step outside of the tile, translate
        if (next.coord.row < 0 || next.coord.row >= TILE ||
            next.coord.col < 0 || next.coord.col >= TILE)
        next = translation[coord.tile][coord.dir](coord);

        // If are about to step on a wall, do not move
        if (tiles[next.tile][next.coord.row][next.coord.col] == '#')
            return coord;
        // otherwise move
        return next;
    }

    int64_t follow_directions() {
        std::stringstream s(directions);
        TiledPosition coord{0, {0,0}, RIGHT};
        while (s) {
            if (std::isdigit(s.peek())) {
                int count;
                s >> count;
                for (int64_t idx : std::views::iota(0, count)) {
                    coord = step(coord);
                }
            } else {
                char turn = s.get();
                if (!s) break;
                if (turn == 'L') coord.dir = coord.dir.left();
                if (turn == 'R') coord.dir = coord.dir.right();
            }
        }

        // Hardcoded for input:
        std::vector<int64_t> row_id = {0, 0, 1, 2, 2, 3};
        std::vector<int64_t> col_id = {1, 2, 1, 0, 1, 0};

        int64_t result = 1000*(row_id[coord.tile]*TILE+coord.coord.row+1) + 4*(col_id[coord.tile]*TILE+coord.coord.col+1);
        if (coord.dir == DOWN) result += 1;
        if (coord.dir == LEFT) result += 2;
        if (coord.dir == UP) result += 3;
        return result;
    }
};

}