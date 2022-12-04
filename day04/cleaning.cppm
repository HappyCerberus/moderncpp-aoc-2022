module;

#include <ranges>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cstdint>
#include <iostream>

export module day04.cleaning;

export namespace day04::cleaning {
struct Interval {
    uint32_t min;
    uint32_t max;

    bool subsumes(const Interval& other) const {
        // A subsumes B if both min and max of B are within A
        return other.min >= min && other.max <= max;
    }
    bool overlaps(const Interval& other) const {
        // A overlaps with B if:
        // - minimum or maximum of B are within A
        // - A is subsumed by B
        return (other.min >= min && other.min <= max) ||
            (other.max >= min && other.max <= max) ||
            other.subsumes(*this);
    }
    friend std::istream& operator>>(std::istream& s, Interval& r) {
        char delim;
        s >> r.min >> delim >> r.max;
        return s;
    }
};

// Part 1: count complete overlap
uint32_t contains(const std::vector<std::pair<Interval,Interval>>& data) {
  	return std::ranges::count_if(data, [](auto& p) {
        return p.first.subsumes(p.second) ||
          p.second.subsumes(p.first);
    });
}

// Part 2: count partial overlap
uint32_t overlaps(const std::vector<std::pair<Interval,Interval>>& data) {
    return std::ranges::count_if(data, [](auto& p) {
        return p.first.overlaps(p.second);
    });
}

}