module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <iostream>

export module day09.bridge;

export namespace day09::bridge {

enum class Direction {
    Right,
    Left,
    Up,
    Down
};

struct Order {
    Direction direction;
    uint32_t count;
    
    friend std::istream& operator>>(std::istream& s, Order& order) {
        char dir;
        if (s >> dir >> order.count) switch (dir) {
            case 'D': order.direction = Direction::Down; break;
            case 'U': order.direction = Direction::Up; break;
            case 'L': order.direction = Direction::Left; break;
            case 'R': order.direction = Direction::Right; break;
            default: throw std::runtime_error("Can't parse Order, wrong direction.");
        }
        return s;
    }
};

struct Position {
    int row;
    int column;
    friend auto operator<=>(const Position&, const Position&) = default;
};
}

template <> struct std::hash<day09::bridge::Position> {
    size_t operator()(const day09::bridge::Position& pos) const {
        return std::hash<int>{}(pos.row) ^ (std::hash<int>{}(pos.column) << 1);
    }
};

export namespace day09::bridge {

void single_step(Position& head, Direction direction) {
    switch (direction) {
        case Direction::Down: head.row--; break;
        case Direction::Up: head.row++; break;
        case Direction::Left: head.column--; break;
        case Direction::Right: head.column++; break;
    }
}

void follow_step(Position& head, Position& tail) {
    if (std::abs(head.row - tail.row) >= 2) {
        tail.row += (head.row - tail.row) / 2;
        if (std::abs(head.column - tail.column) >= 2)
            tail.column += (head.column - tail.column) / 2;
        else
            tail.column += head.column - tail.column;
    } else if (std::abs(head.column - tail.column) >= 2) {
        tail.column += (head.column - tail.column) / 2;
        tail.row += head.row - tail.row;
    }
}

size_t simulate_motions(const std::vector<Order>& orders) {
    Position head{0,0}, tail{0,0};
    std::unordered_set<Position> visited{{0,0}};
    auto single_order = [&](const Order& order) {
        for (auto c : std::views::iota(0uz, order.count)) {
            single_step(head, order.direction);
            follow_step(head, tail);
            visited.insert(tail);
        }
    };
    std::ranges::for_each(orders, single_order);
    return visited.size();
}

size_t simulate_long_motions(const std::vector<Order>& orders) {
    std::vector<Position> rope(10,{0,0});
    std::unordered_set<Position> visited{{0,0}};
    auto single_order = [&](const Order& order) {
        for (auto c : std::views::iota(0uz, order.count)) {
            single_step(rope[0], order.direction);
            // in C++23 we could use std::views::adjacent_view, for now, let's stick with indices
            for (auto idx : std::views::iota(1uz, rope.size()))
                follow_step(rope[idx-1],rope[idx]);
            visited.insert(rope[9]);
        }
    };
    std::ranges::for_each(orders, single_order);
    return visited.size();
}

}