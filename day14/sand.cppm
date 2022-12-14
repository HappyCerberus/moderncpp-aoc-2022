module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <variant>
#include <compare>
#include <sstream>

export module day14.sand;

export namespace day14::sand {
struct Map {
    std::vector<std::vector<char>> map = std::vector<std::vector<char>>(1'000, std::vector<char>(1'000, ' '));
    int min_row = 1'000;
    int max_row = -1;
    int min_col = 1'000;
    int max_col = -1;

    void update_bounding_box(int row, int col) {
        if (min_row > row) min_row = row;
        if (max_row < row) max_row = row;
        if (min_col > col) min_col = col;
        if (max_col < col) max_col = col;
    }

    void add_scan(const std::string& line) {
        std::stringstream s(line);
        int prev_col = -1, prev_row = -1;
        int col = 0, row = 0;
        char delim;
        while (s >> col >> delim >> row) {
            if (prev_col == col)
            for (int idx : std::views::iota(std::min(prev_row, row), std::max(prev_row, row)+1)) {
                map[idx][col] = '#';
                update_bounding_box(idx,col);
            }
            if (prev_row == row)
            for (int idx : std::views::iota(std::min(prev_col, col), std::max(prev_col, col)+1)) {
                map[row][idx] = '#';
                update_bounding_box(row,idx);
            }
            prev_row = row;
            prev_col = col;
            s >> delim >> delim; // process '->'
        }
    }

    bool drop_sand_into_void() {
        int row = 0;
        int col = 500;

        auto step = [&row, &col, this]() {
            // detect when the grain will fall into the void
            if (row+1 > max_row) return false;
            if (col < min_col) return false;
            if (col > max_col) return false;

            // can go down
            if (map[row+1][col] == ' ') { row++; }
            // can go down-left
            else if (map[row+1][col-1] == ' ') { row++; col--; }
            // can go down-right
            else if (map[row+1][col+1] == ' ') { row++; col++; }
            // stuck
            else {
                map[row][col] = 'O';
                return false;
            }

            return true;
        };

        // move until stuck or destined for void
        while (step());
        // return true if stuck
        return map[row][col] == 'O';
    }

    bool drop_sand_onto_floor() {
        if (map[0][500] == 'O') return false;

        int row = 0;
        int col = 500;

        auto step = [&row, &col, this]() {
            if (row < 0 || row >= std::ssize(map) || col < 0 || col >= std::ssize(map[0]))
                throw std::runtime_error("Out of bounds, needs bigger map.");
            
            if (row+1 == max_row+2) {
                map[row][col] = 'O';
                return false;
            } else if (map[row+1][col] == ' ') { row++; }
            else if (map[row+1][col-1] == ' ') { row++; col--; }
            else if (map[row+1][col+1] == ' ') { row++; col++; }
            else {
                map[row][col] = 'O';
                return false;
            }

            return true;
        };

        while (step());
        return map[row][col] == 'O';
    }
}; 
}