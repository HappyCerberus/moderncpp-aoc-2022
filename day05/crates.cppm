module;

#include <ranges>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <iostream>

export module day05.crates;

export namespace day05::crates {

struct Order {
    uint32_t count;
    size_t from;
    size_t to;

    friend auto operator<=>(const Order&, const Order&) = default;
    friend std::istream& operator>>(std::istream& s, Order& o) {
        std::string word;
        s >> word >> o.count >> word >> o.from >> word >> o.to;
        --o.from; // zero-base
        --o.to; // zero-base
        return s;
    }
};

void crateMover9000(std::vector<std::vector<char>>& state, const std::vector<Order>& orders) {
    for (auto &order : orders) {
        // view of the last order.count elements in reverse order
        auto src = state[order.from] | std::views::reverse | std::views::take(order.count);
        std::ranges::copy(src, std::back_inserter(state[order.to]));
        // remove the last order.count elements
        state[order.from].resize(state[order.from].size()-order.count);
    }
}

void crateMover9001(std::vector<std::vector<char>>& state, const std::vector<Order>& orders) {
    for (auto &order: orders) {
        // view of the last order.count elements in order
        auto src = state[order.from] | std::views::reverse | std::views::take(order.count) | std::views::reverse;
        std::ranges::copy(src, std::back_inserter(state[order.to]));
        // remove the last order.count elements
        state[order.from].resize(state[order.from].size()-order.count);
    }
}

std::string top_crates(const std::vector<std::vector<char>>& state) {
    std::string result;
    for (auto &stack : state) {
        if (stack.size() == 0) continue;
        result += stack.back();
    }
    return result;
}

}