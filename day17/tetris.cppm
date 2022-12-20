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

// Verify that a potential cycle is an actual perfect cycle
// perfect cycle:
// - all pieces repeat in their offset positions
// - the pattern advances by the same amount
bool verify_perfect_cycle(std::string pattern, size_t limit, size_t piece_offset, size_t piece_period) {
    // Cannot simulate, total length more than limit
    if (piece_offset + piece_period*10 > limit) return false;

    Chamber chamber;
    size_t pattern_idx = 0;
    size_t shape_idx = 0;

    while (shape_idx != piece_offset) {
        simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        ++shape_idx;
    }

    size_t pattern_base = pattern_idx;
    size_t pattern_diff = 0;

    std::vector<Coord> data;
    // We can check as my repetitions as desired
    for (auto rep : std::views::iota(1, 10)) {
        std::vector<Coord> current;
        // record
        while (shape_idx != piece_offset+rep*piece_period) {
            auto coord = simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
            current.push_back(coord);
            ++shape_idx;
        }
        // check pieces against previous cycle
        if (!data.empty()) {
            int64_t base_old = data[0].row;
            int64_t base_new = current[0].row;
            for (auto idx : std::views::iota(0uz, data.size())) {
                if (data[idx].col != current[idx].col)
                    return false;
                if (data[idx].row-base_old != current[idx].row-base_new)
                    return false;
            }
            data = std::move(current);
            current = {};
        }
        // Verify that the pattern also advances in a regular manner
        if (pattern_diff == 0)
            pattern_diff = pattern_idx-pattern_base;
        if (pattern_idx != pattern_base+rep*pattern_diff)
            return false;
    }

    return true;
}

// Run the simulation with a known cycle
int64_t simulate_with_cycle(std::string pattern, size_t limit, size_t piece_offset, size_t piece_period) {
    Chamber chamber;
    size_t shape_idx = 0;
    size_t pattern_idx = 0;
    while (shape_idx != piece_offset) {
        simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        ++shape_idx;
    }

    // Run through the first instance of the cycle to get the pattern and max_row periods
    size_t base_pattern = pattern_idx;
    size_t base_max_row = chamber.max_row;
    while (shape_idx != piece_offset + piece_period) {
        simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        ++shape_idx;
    }

    size_t pattern_period = pattern_idx - base_pattern;
    size_t max_row_period = chamber.max_row - base_max_row;

    // "simulate" cycles
    size_t cycle_count = ((limit-pattern_idx)/piece_period);
    size_t offset = cycle_count*max_row_period;
    shape_idx += cycle_count*piece_period;
    pattern_idx += cycle_count*pattern_period;

    while (shape_idx != limit) {
        simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        ++shape_idx;
    }
    return chamber.max_row+1+offset;
}

int64_t simulate_with_cycle_detection(std::string pattern, size_t limit) {
    Chamber chamber;
    size_t pattern_idx = 0;
    size_t shape_idx = 0;

    // Remember the column positions of the 5 pieces
    std::multimap<std::vector<int64_t>, int64_t> lookup_table;
    std::vector<int64_t> key;

    while (shape_idx != limit) {
        auto final_coord = simulate_single_piece(chamber, shape_idx, pattern, pattern_idx);
        key.push_back(final_coord.col);
        if (shape_idx % 5 != 4) { // Not the final piece of the set of five
            ++shape_idx;
            continue;
        }

        // For every previously seen configuration that matches current set
        for (auto [it,end] = lookup_table.equal_range(key); it != end; it++)
            // Check if this is actually a cycle
            if (verify_perfect_cycle(pattern, limit, it->second, (shape_idx-4)-it->second))
                // And if it is, simulate
                return simulate_with_cycle(pattern, limit, it->second, (shape_idx-4)-it->second);

        lookup_table.insert(std::make_pair(key,shape_idx-4));
        key = {};
        ++shape_idx;
    }
    
    return -1;
}
}