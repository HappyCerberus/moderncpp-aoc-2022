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

export module day15.sensors;

export namespace day15::sensors {
struct Pos {
    int64_t x;
    int64_t y;
    int64_t distance(const Pos& other) const {
        return std::abs(other.x-x)+std::abs(other.y-y);
    }
};

// Interval from Day 4
struct Interval {
    int64_t min;
    int64_t max;

    bool subsumes(const Interval& other) const {
        return other.min >= min && other.max <= max;
    }
    bool overlaps(const Interval& other) const {
        return (other.min >= min && other.min <= max) ||
            (other.max >= min && other.max <= max) ||
            other.subsumes(*this);
    }

    // New operations for Day 15
    friend auto operator <=> (const Interval&, const Interval&) = default;
    Interval join(const Interval& other) const {
        return Interval{std::min(other.min, min), std::max(other.max, max)};
    }
    int64_t length() const { return max-min; }
};

struct Sensor {
    Pos position;
    Pos beacon;
    // Potentially empty interval that is covered by this beacon at a given y coordinate
    std::optional<Interval> coverage_at_row(int64_t y_coord) const {
        int64_t beacon_distance = position.distance(beacon);
        if (std::abs(position.y - y_coord) > beacon_distance) return std::nullopt;
        return Interval{
            position.x - (beacon_distance - std::abs(position.y - y_coord)), 
            position.x + (beacon_distance - std::abs(position.y - y_coord))
        };
    }
};

std::vector<Interval> coverage_at_row(const std::vector<Sensor>& sensors, int64_t y_coord) {
    // Go over all sensors, get their intervals, and filter out empty ones.
    auto has_value = [](const auto &opt) { return opt.has_value(); };
    auto get_value = [](const auto &opt) { return *opt; };
    auto coverage_at = [y_coord](const auto &sensor) {
        return sensor.coverage_at_row(y_coord);
    };
    std::vector<Interval> intervals;
    std::ranges::transform(sensors | std::views::transform(coverage_at) | std::views::filter(has_value), std::back_inserter(intervals), get_value);
    if (intervals.empty()) return {};

    // Sort the intervals and then merge them
    std::ranges::sort(intervals);
    std::vector<Interval> merged;
    
    auto it = intervals.begin();
    Interval prev = *it;
    ++it;
    while (it != intervals.end()) {
        Interval curr = *it;
        if (prev.overlaps(curr)) {
            prev = prev.join(curr);
        } else {
            merged.push_back(prev);
            prev = curr;
        }
        ++it;
    }
    merged.push_back(prev);
    return merged;
}

int64_t count_covered_spaces_at_coord(const std::vector<Sensor>& sensors, int64_t y_coord) {
    std::vector<Interval> result = coverage_at_row(sensors, y_coord);
    return std::reduce(result.begin(), result.end(), UINT64_C(0), [](int64_t acc, const Interval& it) {
        return acc + it.length();
    });
}

int64_t tuning_frequency(const std::vector<Sensor>& sensors, int64_t min, int64_t max) {
    for (int64_t y = min; y <= max; y++) {
        auto v = coverage_at_row(sensors, y);
        // Is there a gap?
        if (v.size() > 1)
            return (v[0].max+1)*4000000+y;
    }
    return 0;
}
}