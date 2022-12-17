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

export module day17.tetris;

export namespace day17::tetris {
struct Coord {
    int64_t row;
    int64_t col;
    friend auto operator<=>(const Coord&, const Coord&) = default;
};

struct Chamber {
    Chamber() : map(7, std::vector<char>(7, ' ')) {}

    // Helper to check if a coordinate is empty (and not out of bounds)
    bool is_empty(const Coord& coord) const {
        if (coord.row < 0) return false;
        if (coord.col < 0 || coord.col >= 7) return false;
        return map[coord.row][coord.col] == ' ';
    }
    // Called after a piece is dropped to update the maximum height and keep enough space for new pieces
    void update_max_row(int64_t max) {
        if (max > max_row)
            max_row = max;
        while (std::ssize(map) <= max_row+7)
            map.push_back(std::vector<char>(7, ' '));
    }
    int64_t max_row = -1;
    std::vector<std::vector<char>> map;

    friend std::ostream& operator<<(std::ostream& s, const Chamber& chamber) {
        s << "(-------)\n";
        for (auto &row : chamber.map) {
            s << "(";
            for (auto &col : row) {
                s << col;
            }
            s << ")\n";
        }
        return s;
    }
};

// Horizontal piece with collision detection
struct Horizontal {
    Horizontal(int64_t max_row) : coord{max_row+4,2} {}
    bool left(const Chamber& chamber) {
        if (!chamber.is_empty({coord.row, coord.col-1}))
            return false;
        coord.col--;
        return true;
    }
    bool right(const Chamber& chamber) {
        if (!chamber.is_empty({coord.row, coord.col+4}))
            return false;
        coord.col++;
        return true;
    }
    bool down(const Chamber& chamber) {
        for (auto idx : std::views::iota(0,4))
            if (!chamber.is_empty({coord.row-1, coord.col+idx}))
                return false;
        --coord.row;
        return true;
    }
    int64_t max_row() const {
        return coord.row;
    }
    std::vector<Coord> shape() const {
        std::vector<Coord> result;
        for (auto idx : std::views::iota(0,4))
            result.push_back({coord.row, coord.col+idx});
        return result;
    }
    Coord coord; // left edge coordinate
};

// Plus shape with collision detection
struct Plus {
    Plus(int64_t max_row) : coord{max_row+5,3} {}
    bool left(const Chamber& chamber) {
        // bottom of the plus
        if (!chamber.is_empty({coord.row-1, coord.col-1}))
            return false;
        // left-side of the plus
        if (!chamber.is_empty({coord.row, coord.col-2}))
            return false;
        --coord.col;
        return true;
    }
    bool right(const Chamber& chamber) {
        // bottom of the plus
        if (!chamber.is_empty({coord.row-1, coord.col+1}))
            return false;
        // right-side of the plus
        if (!chamber.is_empty({coord.row, coord.col+2}))
            return false;
        ++coord.col;
        return true;
    }
    bool down(const Chamber& chamber) {
        // bottom of the plus
        if (!chamber.is_empty({coord.row-2, coord.col}))
            return false;
        // left-side of the plus
        if (!chamber.is_empty({coord.row-1, coord.col-1}))
            return false;
        // right-side of the plus
        if (!chamber.is_empty({coord.row-1, coord.col+1}))
            return false;
        --coord.row;
        return true;
    }
    int64_t max_row() const {
        return coord.row+1;
    }
    std::vector<Coord> shape() const {
        std::vector<Coord> result;
        result.push_back(coord);
        result.push_back({coord.row, coord.col-1});
        result.push_back({coord.row, coord.col+1});
        result.push_back({coord.row+1, coord.col});
        result.push_back({coord.row-1, coord.col});
        return result;
    }
    Coord coord; // middle coord
};

// Corner shape with collision detection
struct Corner {
    Corner(int64_t max_row) : coord{max_row+4,4} {}
    bool left(const Chamber& chamber) {
        if (!chamber.is_empty({coord.row, coord.col-3}))
            return false;
        --coord.col;
        return true;
    }
    bool right(const Chamber& chamber) {
        for (auto idx : std::views::iota(0, 3))
            if (!chamber.is_empty({coord.row+idx, coord.col+1}))
                return false;
        ++coord.col;
        return true;
    }
    bool down(const Chamber& chamber) {
        for (auto idx : std::views::iota(0, 3))
            if (!chamber.is_empty({coord.row-1, coord.col-idx}))
                return false;
        --coord.row;
        return true;
    }
    int64_t max_row() const {
        return coord.row+2;
    }
    std::vector<Coord> shape() const {
        std::vector<Coord> result;
        result.push_back(coord);
        for (auto idx : std::views::iota(1, 3)) {
            result.push_back({coord.row+idx, coord.col});
            result.push_back({coord.row, coord.col-idx});
        }
        return result;
    }
    Coord coord; // corner coord
};

// Vertical shape with collision detection
struct Vertical {
    Vertical(int64_t max_row) : coord{max_row+4,2} {}
    bool left(const Chamber& chamber) {
        for (auto idx : std::views::iota(0,4))
            if (!chamber.is_empty({coord.row+idx, coord.col-1}))
                return false;
        coord.col--;
        return true;
    }
    bool right(const Chamber& chamber) {
        for (auto idx : std::views::iota(0,4))
            if (!chamber.is_empty({coord.row+idx, coord.col+1}))
                return false;
        coord.col++;
        return true;
    }
    bool down(const Chamber& chamber) {
        if (!chamber.is_empty({coord.row-1, coord.col}))
            return false;
        --coord.row;
        return true;
    }
    int64_t max_row() const {
        return coord.row+3;
    }
    std::vector<Coord> shape() const {
        std::vector<Coord> result;
        for (auto idx : std::views::iota(0,4))
            result.push_back({coord.row+idx, coord.col});
        return result;
    }
    Coord coord; // bottom edge coordinate
};

// Square shape with collision detection
struct Square {
    Square(int64_t max_row) : coord{max_row+4,2} {}
    bool left(const Chamber& chamber) {
        if (!chamber.is_empty({coord.row, coord.col-1}))
            return false;
        if (!chamber.is_empty({coord.row+1, coord.col-1}))
            return false;
        --coord.col;
        return true;
    }
    bool right(const Chamber& chamber) {
        if (!chamber.is_empty({coord.row, coord.col+2}))
            return false;
        if (!chamber.is_empty({coord.row+1, coord.col+2}))
            return false;
        ++coord.col;
        return true;
    }
    bool down(const Chamber& chamber) {
        if (!chamber.is_empty({coord.row-1, coord.col}))
            return false;
        if (!chamber.is_empty({coord.row-1, coord.col+1}))
            return false;
        --coord.row;
        return true;
    }
    int64_t max_row() const {
        return coord.row+1;
    }
    std::vector<Coord> shape() const {
        std::vector<Coord> result;
        result.push_back(coord);
        result.push_back({coord.row, coord.col+1});
        result.push_back({coord.row+1, coord.col});
        result.push_back({coord.row+1, coord.col+1});
        return result;
    }
    Coord coord; // left-bottom corner
};

// A variant type to store all shapes
using AllShapes = std::variant<Horizontal, Plus, Corner, Vertical, Square>;

// Draw any shape by getting its coordinates
void draw(Chamber& chamber, const AllShapes& shape) {
    auto coords = std::visit([](auto &s) { return s.shape(); }, shape);
    for (auto c : coords) {
        chamber.map[c.row][c.col] = '@';
    }
}

// Generate a new shape based on the shape index
// the current max_row determines where it spawns
AllShapes get_shape(int64_t id, int64_t max_row) {
    switch (id % 5) {
        case 0: return Horizontal(max_row);
        case 1: return Plus(max_row);
        case 2: return Corner(max_row);
        case 3: return Vertical(max_row);
        case 4: return Square(max_row);
    }
    // unreachable
    return Horizontal(max_row);
}


Coord simulate_single_piece(Chamber &chamber, size_t shape_idx, const std::string& pattern, size_t &pattern_idx) {
    // Get the piece
    AllShapes current_shape = get_shape(shape_idx, chamber.max_row);
    while (true) {
        // Move to the side
        if (pattern[pattern_idx % pattern.length()] == '<')
            std::visit([&chamber](auto &s) { s.left(chamber); }, current_shape);
        else if (pattern[pattern_idx % pattern.length()] == '>')
            std::visit([&chamber](auto &s) { s.right(chamber); }, current_shape);
        ++pattern_idx;

        // And then down, if down returns false, we know we are stuck
        if (!std::visit([&chamber](auto &s) { return s.down(chamber); }, current_shape)) {
            // Draw the piece into the chamber
            draw(chamber, current_shape);
            // Update the maximum height, based on the shape
            chamber.update_max_row(std::visit([](const auto &s) { return s.max_row(); }, current_shape));
            // Return the coordinate
            return std::visit([](const auto &s) { return s.coord; }, current_shape);
        }
    }
    // unreachable
    return {0,0};
}

// Entire simulation
int64_t simulate(std::string pattern, size_t limit) {
    Chamber chamber;
    size_t pattern_idx = 0;
    size_t shape_idx = 0;
    // Until we reach the limit of pieces dropped, simulate one piece at a time
    while (shape_idx != limit) {
        simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        ++shape_idx;
    }
    return chamber.max_row+1;
}

struct CycleInfo {
    size_t shape_idx;
    size_t pattern_idx;
    int64_t max;
    std::vector<int64_t> rows;
    bool is_cycle(const CycleInfo& other) {
        return rows[1]-rows[0] == other.rows[1]-other.rows[0] &&
            rows[2]-rows[0] == other.rows[2]-other.rows[0] &&
            rows[3]-rows[0] == other.rows[3]-other.rows[0] &&
            rows[4]-rows[0] == other.rows[4]-other.rows[0];
    }
};

// Incorrect solution
int64_t simulate_with_cycle_detection(std::string pattern, size_t limit) {
    Chamber chamber;
    size_t pattern_idx = 0;
    size_t shape_idx = 0;

    // First, detect cycle
    std::multimap<std::vector<int64_t>, CycleInfo> lookup_table;
    std::vector<int64_t> key;
    CycleInfo value;

    size_t initial_limit = 0;
    size_t cycle_period = 0;
    size_t pattern_period = 0;
    int64_t max_row_diff = 0;

    while (shape_idx != limit) {
        if (shape_idx % 5 == 0) {
            value.shape_idx = shape_idx;
            value.pattern_idx = pattern_idx;
        }

        auto final_coord = simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        key.push_back(final_coord.col);
        value.rows.push_back(final_coord.row);

        if (shape_idx % 5 == 4) { 
            value.max = chamber.max_row;
            for (auto [it,end] = lookup_table.equal_range(key); it != end; it++) {
                if (value.is_cycle(it->second)) {
                    initial_limit = shape_idx-4;
                    cycle_period = (shape_idx-4)-it->second.shape_idx;
                    max_row_diff = value.max-it->second.max;
                    pattern_period = value.pattern_idx-it->second.pattern_idx;
                    break;
                }
            }
            if (initial_limit != 0) break;
            lookup_table.insert(std::make_pair(key,value));
            key = {};
            value = {};
        }
        ++shape_idx;
    }
    
    chamber = {};
    shape_idx = 0;
    pattern_idx = 0;
    while (shape_idx != initial_limit) {
        simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        ++shape_idx;
    }

    // "simulate" cycles
    size_t cycle_count = ((limit-initial_limit)/cycle_period);
    size_t offset = cycle_count*max_row_diff;

    shape_idx += cycle_count*cycle_period;
    pattern_idx += cycle_count*pattern_period;
    while (shape_idx != limit) {
        simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        ++shape_idx;
    }
    return chamber.max_row+1+offset;
}
}