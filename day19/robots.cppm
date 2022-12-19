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
#include <queue>
#include <ranges>
#include <map>

export module day19.robots;

export namespace day19::robots {
struct Blueprint {
    int32_t ore_robot_cost;
    int32_t clay_robot_cost;
    int32_t obsidian_robot_base_cost;
    int32_t obsidian_robot_clay_cost;
    int32_t geode_robot_base_cost;
    int32_t geode_robot_obsidian_cost;

    friend std::istream& operator>>(std::istream& s, Blueprint& blueprint) {
        std::string w;
        // Blueprint x: Each ore robot costs
        for (auto idx : std::views::iota(0, 6))
            s >> w;
        s >> blueprint.ore_robot_cost;
        // ore. Each clay robot costs
        for (auto idx : std::views::iota(0,5))
            s >> w;
        s >> blueprint.clay_robot_cost;
        // ore. Each obsidian robot costs
        for (auto idx : std::views::iota(0,5))
            s >> w;
        s >> blueprint.obsidian_robot_base_cost >> w >> w >> blueprint.obsidian_robot_clay_cost;
        // clay. Each geode robot costs
        for (auto idx : std::views::iota(0,5))
            s >> w;
        s >> blueprint.geode_robot_base_cost >> w >> w >> blueprint.geode_robot_obsidian_cost >> w;
        return s;
    }
};

struct State {
    int32_t ore_robots;
    int32_t clay_robots;
    int32_t obsidian_robots;
    int32_t geode_robots;

    int32_t time;

    int32_t ore;
    int32_t clay;
    int32_t obsidian;
    int32_t geodes;

    void mine() {
        ore += ore_robots;
        clay += clay_robots;
        obsidian += obsidian_robots;
        geodes += geode_robots;
        ++time;
    }

    bool can_build_geode_robot(const Blueprint& bp) const {
        return obsidian >= bp.geode_robot_obsidian_cost &&
            ore >= bp.geode_robot_base_cost;
    }
    void build_geode_robot(const Blueprint& bp) {
        obsidian -= bp.geode_robot_obsidian_cost;
        ore -= bp.geode_robot_base_cost;
        ++geode_robots;
    }

    bool can_build_obsidian_robot(const Blueprint& bp) const {
        return clay >= bp.obsidian_robot_clay_cost &&
            ore >= bp.obsidian_robot_base_cost;
    }
    void build_obsidian_robot(const Blueprint& bp) {
        clay -= bp.obsidian_robot_clay_cost;
        ore -= bp.obsidian_robot_base_cost;
        ++obsidian_robots;
    }
    bool makes_sense_to_build_obsidian_robot(const Blueprint& bp) const {
        return obsidian_robots < bp.geode_robot_obsidian_cost;
    }

    bool can_build_clay_robot(const Blueprint& bp) const {
        return ore >= bp.clay_robot_cost;
    }
    void build_clay_robot(const Blueprint& bp) {
        ore -= bp.clay_robot_cost;
        ++clay_robots;
    }
    bool makes_sense_to_build_clay_robot(const Blueprint& bp) const {
        return clay_robots < bp.obsidian_robot_clay_cost;
    }

    bool can_build_ore_robot(const Blueprint& bp) const {
        return ore >= bp.ore_robot_cost;
    }
    void build_ore_robot(const Blueprint& bp) {
        ore -= bp.ore_robot_cost;
        ++ore_robots;
    }
    bool makes_sense_to_build_ore_robot(const Blueprint& bp) const {
        return ore_robots < std::max({bp.geode_robot_base_cost, bp.obsidian_robot_base_cost, bp.clay_robot_cost});
    }
};



int32_t bfs(Blueprint blueprint, int32_t max_time) {
    std::queue<State> q;
    q.push(State{1,0,0,0,0,0,0,0,0});

    int32_t max_geodes = 0;
    while (!q.empty()) {
        State current = q.front();
        q.pop();

        auto should_build_something = [](const Blueprint& bp, const State& state) {
            return state.can_build_geode_robot(bp) ||
            (state.can_build_obsidian_robot(bp) && state.makes_sense_to_build_obsidian_robot(bp)) ||
            (state.can_build_clay_robot(bp) && state.makes_sense_to_build_clay_robot(bp)) ||
            (state.can_build_ore_robot(bp) && state.makes_sense_to_build_ore_robot(bp));
        };

        // fast-forward
        while (current.time != max_time && !should_build_something(blueprint, current))
            current.mine();

        // Terminal state
        // No point in build anything at max_time-1 since the robot wouldn't contribute
        if (current.time == max_time-1)
            current.mine();
        // Done, update maximum
        if (current.time == max_time) {
            max_geodes = std::max(max_geodes, current.geodes);
            continue;
        }

        // Building a geode robot is always good
        if (current.can_build_geode_robot(blueprint)) {
            current.mine();
            current.build_geode_robot(blueprint);
            q.push(current);
            continue; // do not expand search space
        }

        // I think, this is actually not correct, it could be sub-optimal to greedily build obsidian robots
        // but this alone makes a difference of producing a result in ~10 seconds and running out of memory
        // and most importantly, it's fine for my input.
        if (current.can_build_obsidian_robot(blueprint) && 
            current.makes_sense_to_build_obsidian_robot(blueprint)) {
            current.mine();
            current.build_obsidian_robot(blueprint);
            q.push(current);
            continue; // do not expand search space
        }

        if (current.can_build_clay_robot(blueprint) &&
            current.makes_sense_to_build_clay_robot(blueprint)) {
            State clay_robot = current;
            clay_robot.mine();
            clay_robot.build_clay_robot(blueprint);
            q.push(clay_robot);
        }
        if (current.can_build_ore_robot(blueprint) &&
            current.makes_sense_to_build_ore_robot(blueprint)) {
            State ore_robot = current;
            ore_robot.mine();
            ore_robot.build_ore_robot(blueprint);
            q.push(ore_robot);
        }
        current.mine();
        q.push(current);
    }
    return max_geodes;
}

int32_t quality_levels(const std::vector<Blueprint>& blueprints, int32_t max_time) {
    int32_t total = 0;
    for (auto idx : std::views::iota(0z,std::ssize(blueprints))) {
        int32_t result = bfs(blueprints[idx], max_time);
        total += result*(idx+1);
    }
    return total;
}

}