module;

#include <ranges>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cstdint>

export module day03.rucksacks;

export namespace day03::rucksacks {

uint64_t overlap(const std::vector<std::string>& rucksacks) {
    // Take a copy as we iterate, so we can mutate the line.
    auto value = [](std::string in) -> uint64_t {
        // Get the midpoint.
        auto it = in.begin() + in.length()/2;

        // Sort the lower and upper parts.
        std::sort(in.begin(), it);
        std::sort(it, in.end());

        // Calculate the intersection, which is the common item.
        std::vector<char> out;
        std::set_intersection(
            in.begin(), it, // lower part
            it, in.end(),   // upper part
            std::back_inserter(out));

        // Note that the intersection can be multiple items,s
        // however, always only one category of an item. We
        // could technically use std::unique to narrow it
        // down to a single item, but who has the time.
        if (out.size() == 0) std::abort();
        // Translate into "priority value".
        if (std::isupper(out[0])) {
            return 27 + out[0]-'A';
        }
        return 1 + out[0]-'a';
    };
    // Finally, sum up the results for each rucksack.
    auto rng = rucksacks | std::views::transform(value) | std::views::common;
    return std::reduce(rng.begin(), rng.end());
}

// Take the input by-copy since we are modifying it
uint64_t badges(std::vector<std::string> rucksacks) {
    // pre-sort the input
    std::ranges::for_each(rucksacks, std::ranges::sort);

    uint64_t result = 0;
    // We will be getting a chunk_view in C++23, 
    // for now, we are stuck with indices.
    for (size_t i = 0; i+2 < rucksacks.size(); i+=3) {
        // Determine the common items in the first two rucksacks.
        std::vector<char> tmp;
        std::ranges::set_intersection(rucksacks[i], rucksacks[i+1], std::back_inserter(tmp));
        // Determine the common items between the common items and the third rucksack.
        std::vector<char> out;
        std::ranges::set_intersection(tmp, rucksacks[i+2], std::back_inserter(out));
        
        // As with part1, we can have multiple copies of the same item.
        if (out.size() == 0) std::abort();
        // Calculate priority and add to the total.
        if (std::isupper(out[0])) {
            result += 27 + out[0]-'A';
        } else {
            result += 1 + out[0]-'a';
        }
    }
    return result;
}

}