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

export module day23.elves;

export namespace day23::elves {

struct Map;
struct ElfLogic;

struct Elf {
    int64_t row;
    int64_t col;
    // The next position based on the current elf logic
    Elf next(const Map& lookup, const ElfLogic& logic) const;
    friend auto operator<=>(const Elf&, const Elf&) = default;
};

// Encoding of single elf-logic piece
struct DirectedMove {
    // Checks if the cardinal direction is empty
    std::function<bool(const Map& lookup, Elf elf)> is_empty;
    // Returns the modified position
    std::function<Elf(Elf elf)> get;
};

// Elf-logic storage
struct ElfLogic {
    // The four pieces of the logic
    std::vector<DirectedMove> moves;
    // offset that will change each round
    int offset;
    ElfLogic();
    // Apply elf-logic and get new position for the elf
    Elf operator()(const Map& lookup, Elf elf) const;
    // Rotate the logic
    void rotate();
};

struct BoundingBox {
    int64_t min_row;
    int64_t min_col;
    int64_t max_row;
    int64_t max_col;
};

// Map provides storage for elf positions and helper functions for moving elves
struct Map {
    void record(Elf elf);
    void move(Elf from, Elf to);
    bool has_elf(int64_t row, int64_t col) const;
    BoundingBox get_bounding_box() const;

    // Sparse storage
    std::unordered_map<int64_t, std::unordered_set<int64_t>> position_lookup;
};

struct Elves {
    std::vector<Elf> elves;
    Map map;
    ElfLogic logic;

    // One round, false if no elf moved
    bool round();
    // Number of empty tiles in the boudning box
    int64_t empty_tiles() const;


    friend std::ostream& operator<<(std::ostream& s, Elves& elves) {
        BoundingBox box = elves.map.get_bounding_box();
        for (int64_t ridx : std::views::iota(box.min_row, box.max_row+1)) {
            for (int64_t cidx : std::views::iota(box.min_col, box.max_col+1)) {
                if (elves.map.has_elf(ridx, cidx))
                    s << '#';
                else
                    s << '.';
            }
            s << "\n";
        }
        return s;
    }
    friend std::istream& operator>>(std::istream& s, Elves& elves) {
        std::string line;
        int64_t ridx = 0;
        while (getline(s, line)) {
            for (int64_t cidx : std::views::iota(0z, std::ssize(line))) {
                if (line[cidx] != '#') continue;
                elves.elves.push_back(Elf{ridx,cidx});
                elves.map.record(Elf{ridx, cidx});
            }
            ++ridx;
        }
        return s;
    }
};

Elf Elf::next(const Map& lookup, const ElfLogic& logic) const {
    return logic(lookup, *this);
}

// Setup the four pieces of elf logic, moving in each cardinal direction
ElfLogic::ElfLogic() : moves{
    // No elves in N, NE, NW
    {[](const Map& lookup, Elf elf){
        return !(lookup.has_elf(elf.row-1, elf.col) || 
            lookup.has_elf(elf.row-1, elf.col-1) || 
            lookup.has_elf(elf.row-1, elf.col+1));
    },
    [](Elf elf) { return Elf{elf.row-1, elf.col}; }},
    // No elves in S, SE, SW
    {[](const Map& lookup, Elf elf){
        return !(lookup.has_elf(elf.row+1, elf.col) || 
            lookup.has_elf(elf.row+1, elf.col-1) || 
            lookup.has_elf(elf.row+1, elf.col+1));
    },
    [](Elf elf) { return Elf{elf.row+1, elf.col}; }},
    // No elves in W, NW, SW
    {[](const Map& lookup, Elf elf){
        return !(lookup.has_elf(elf.row, elf.col-1) || 
            lookup.has_elf(elf.row-1, elf.col-1) || 
            lookup.has_elf(elf.row+1, elf.col-1));
    },
    [](Elf elf) { return Elf{elf.row,elf.col-1}; }},
    // No elves in E, NE, SE
    {[](const Map& lookup, Elf elf) {
        return !(lookup.has_elf(elf.row, elf.col+1) || 
            lookup.has_elf(elf.row-1, elf.col+1) || 
            lookup.has_elf(elf.row+1, elf.col+1));
    },
    [](Elf elf) { return Elf{elf.row, elf.col+1}; }}}, 
    offset(0) {}

Elf ElfLogic::operator()(const Map& lookup, Elf elf) const {
    // Count elves around, if none, do not move
    int sum = 0;
    for (int r : {-1,0,1}) 
        for (int c : {-1,0,1})
            if (lookup.has_elf(elf.row+r,elf.col+c))
                ++sum;
    if (sum == 1) return elf;

    // iterate over the four cardinal directions in current order
    for (int idx : std::views::iota(0,4))
        if (moves[(offset+idx)%4].is_empty(lookup, elf))
            return moves[(offset+idx)%4].get(elf);

    // Nowhere to move
    return elf;
}

// Adjust the logic offset for the next round
void ElfLogic::rotate() {
    ++offset;
    offset %= 4;
}


void Map::record(Elf elf) {
    position_lookup[elf.row].insert(elf.col);
}

void Map::move(Elf from, Elf to) {
    position_lookup[from.row].erase(from.col);
    position_lookup[to.row].insert(to.col);
}

bool Map::has_elf(int64_t row, int64_t col) const {
    return position_lookup.contains(row) && position_lookup.at(row).contains(col);
}

BoundingBox Map::get_bounding_box() const {
    // libc++ is missing std::views::keys, std::views::values and std::ranges::min/max
    // so, insted we simply iterate over all stored coordinates
    BoundingBox box{
        INT64_MAX,
        INT64_MAX,
        INT64_MIN,
        INT64_MIN
    };
    for (auto &row : position_lookup) {
        box.min_row = std::min(box.min_row, row.first);
        box.max_row = std::max(box.max_row, row.first);
        for (auto &col : row.second) {
            box.min_col = std::min(box.min_col, col);
            box.max_col = std::max(box.max_col, col);
        }
    }
    return box;
}

bool Elves::round() {
    std::unordered_map<int64_t, std::unordered_map<int64_t, int64_t>> plans;
    std::vector<Elf> nexts;

    bool moved = false;
    // Each elf plans their move
    for (auto &elf : elves) {
        Elf next = elf.next(map, logic);
        ++plans[next.row][next.col];
        nexts.push_back(next);
    }

    // Each elf tries to execute
    for (size_t idx : std::views::iota(0uz, elves.size())) {
        Elf next = nexts[idx];
        // Only this elf is moving to this position and it's an actual move
        if (plans[next.row][next.col] == 1 && elves[idx] != next) {
            map.move(elves[idx], next);
            moved = true;
            elves[idx] = next;
        }
    }
    
    // Rotate the logic for next round
    logic.rotate();
    return moved;
}

// Count the empty tiles
int64_t Elves::empty_tiles() const {
    int64_t result = 0;
    BoundingBox box = map.get_bounding_box();
    for (int64_t ridx : std::views::iota(box.min_row, box.max_row+1))
        for (int64_t cidx : std::views::iota(box.min_col, box.max_col+1))
            if (!map.has_elf(ridx, cidx)) // empty
                result++;
    return result;
}

}