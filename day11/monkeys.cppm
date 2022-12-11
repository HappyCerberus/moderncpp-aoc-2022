module;

#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_set>
#include <iostream>

export module day11.monkeys;

export namespace day11::monkeys {

struct Monkeys;
struct Monkey {
    std::vector<int64_t> items;
    int64_t (*inspect)(int64_t);
    int64_t (*test)(int64_t);
    Monkeys* troop;
    int64_t divisor = 3;
    int64_t counter = 0;
    void inspect_items();
    void worried_inspect_items();
};

struct Monkeys {
    std::vector<Monkey> troop;
    void throw_item(int64_t item, int64_t monkey) {
        troop[monkey].items.push_back(item);
    }
    int64_t monkey_business() {
        std::ranges::sort(troop, std::greater<>{}, &Monkey::counter);
        return troop[0].counter*troop[1].counter;
    }
    void round() {
        std::ranges::for_each(troop, &Monkey::inspect_items);
    }
    void worried_round() {
        std::ranges::for_each(troop, &Monkey::worried_inspect_items);
    }
};

void Monkey::inspect_items() {
    counter += items.size();
    std::ranges::transform(items, items.begin(), inspect);
    std::ranges::transform(items, items.begin(), [this](int64_t v) { return v / divisor; });
    for (auto v : items)
        troop->throw_item(v, test(v));
    items.clear();
}

void Monkey::worried_inspect_items() {
    counter += items.size();
    std::ranges::transform(items, items.begin(), inspect);
    std::ranges::transform(items, items.begin(), [this](int64_t v) { return v % divisor; });
    for (auto v : items)
        troop->throw_item(v, test(v));
    items.clear();
}

}