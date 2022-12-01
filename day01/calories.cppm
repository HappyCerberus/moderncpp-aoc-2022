module;

#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <iostream>

export module day01.calories;

export namespace day01::calories {

uint64_t max_calories(const std::vector<std::string>& data) {
    auto by_elf = data |
        std::views::lazy_split(std::string{}) | // group by elf: range{range{string}}
        std::views::transform([](const auto& elf) -> uint64_t { // sum up the calories for each elf: range{uint64_t}
            auto to_unsigned = [](const auto& in) { return std::stoull(in); };
            auto rng = elf | std::views::transform(to_unsigned) | std::views::common; // range{string} -> range{uint64_t}
            return std::reduce(rng.begin(), rng.end()); // range{uint64_t} -> uint64_t
        });
    return std::ranges::max(by_elf);
}

uint64_t top_three(const std::vector<std::string>& data) {
    auto by_elf = data |
        std::views::lazy_split(std::string{}) | // group by elf: range{range{string}}
        std::views::transform([](const auto& elf) -> uint64_t { // sum up the calories for each elf: range{uint64_t}
            auto to_unsigned = [](const auto& in) { return std::stoull(in); };
            auto rng = elf | std::views::transform(to_unsigned) | std::views::common; // range{string} -> range{uint64_t}
            return std::reduce(rng.begin(), rng.end()); // range{uint64_t} -> uint64_t
        });
    std::vector<uint64_t> top(3);
    std::ranges::partial_sort_copy(by_elf, top, std::greater<>{});
    return std::reduce(top.begin(), top.end());
}

}