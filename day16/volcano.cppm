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

export module day16.volcano;

export namespace day16::volcano {
struct Layout {
    // List of valves
    std::unordered_set<std::string> valves;
    // Flow rate for each valve
    std::unordered_map<std::string, int64_t> flow_rate;
    // Neighbourhood list
    std::unordered_multimap<std::string, std::string> tunnels;
    // Valves that have non-zero throughput
    std::unordered_set<std::string> useful_valves;
    // All-to-all shortest paths calculated using Floyd-Warshall
    std::unordered_map<std::string, std::unordered_map<std::string, int64_t>> all_to_all;

    auto floyd_warshall() {
        std::unordered_map<std::string, std::unordered_map<std::string, int64_t>> distance;
        for (auto &from : valves) for (auto &to : valves) distance[from][to] = valves.size()+1;

        // known paths
        for (auto &[from,to] : tunnels) distance[from][to] = 1;
        for (auto &from : valves) distance[from][from] = 0;
        // paths that go via another node
        for (auto &via : valves) for (auto &from : valves) for (auto &to : valves)
            if (distance[from][to] > distance[from][via] + distance[via][to])
                distance[from][to] = distance[from][via] + distance[via][to];
        return distance;
    }


    int64_t get_flow(const std::unordered_set<std::string>& valves) {
        return std::accumulate(valves.begin(), valves.end(), INT64_C(0), 
            [this](int64_t acc, auto &valve) { return acc+flow_rate[valve]; });
    }

    int64_t dfs(std::string current, int64_t time, int64_t total, std::unordered_set<std::string>& open) {
        // If we do nothing the maximum is: current total + flow from all opened valves * remaining time
        int64_t max = total + get_flow(open)*(30-time);
        // Try to open more valves
        for (auto &next : useful_valves) {
            // moving to this valve is useless, as it is already open
            if (open.contains(next))
                continue;
            
            int64_t time_delta = all_to_all[current][next] + 1;
            // moving to this valve and opening it would take more time than we have
            if (time + time_delta >= 30)
                continue;

            // the flow as we move to the next valve and open it
            int64_t new_total = total + time_delta*get_flow(open);
            open.insert(next);
            // recurse with this valve open, if it is an improvement, remember
            int64_t value = dfs(next, time + time_delta, new_total, open);
            if (max < value) max = value;
            open.erase(next);
        }
        return max;
    }

    int64_t dfs_elephant(std::string current, bool elephant, int64_t time, int64_t total, std::unordered_set<std::string>& open, const std::unordered_set<std::string>& useful) {
        // If we do nothing the maximum is: current total + flow from all opened valves * remaining time
        int64_t max = total + get_flow(open)*(26-time);
        // But if we are us, we can let loose an elephant
        if (!elephant) {
            // The elephant can only open valves that we haven't open yet.
            std::unordered_set<std::string> new_candidates = useful;
            for (auto &v : open) new_candidates.erase(v);

            std::unordered_set<std::string> new_open;
            // Let the elephant run around from "AA" at time zero
            int64_t max_elephant = dfs_elephant("AA", true, 0, 0, new_open, new_candidates);
            max = total + get_flow(open)*(26-time) + max_elephant;
        }
        for (auto &next : useful) {
            // moving to this valve is useless, as it is already open
            if (open.contains(next))
                continue;

            int64_t time_delta = all_to_all[current][next] + 1;
            // moving to this valve and opening it would take more time than we have
            if (time + time_delta >= 26)
                continue;

            // the flow as we move to the next valve and open it
            int64_t new_total = total + time_delta*get_flow(open);
            open.insert(next);
            // recurse with this valve open, if it is an improvement, remember
            // importantly, this path is common for both the us and the elephant
            int64_t value = dfs_elephant(next, elephant, time + time_delta, new_total, open, useful);
            if (max < value) max = value;
            open.erase(next);
        }
        return max;
    }

    int64_t dfs() {
        std::unordered_set<std::string> open;
        return dfs("AA", 0, 0, open);
    }

    int64_t dfs_elephant() {
        std::unordered_set<std::string> open;
        return dfs_elephant("AA", false, 0, 0, open, useful_valves);
    }

    friend std::istream& operator>>(std::istream& s, Layout& layout) {
        std::string node;
        int64_t flow;
        while (s) {
            s.ignore(6);
            s >> node;
            s.ignore(15);
            s >> flow;
            layout.valves.insert(node);
            layout.flow_rate[node] = flow;

            char delim;
            s >> delim;
            std::string skip;
            s >> skip >> skip >> skip >> skip;
            std::string tunnel;
            while (s >> tunnel) {
                bool last = (tunnel[tunnel.length()-1] != ',');
                if (!last) tunnel.resize(2);
                layout.tunnels.insert(std::make_pair(node, tunnel));
                if (last) break;
            }
        }

        for (auto &v : layout.flow_rate | std::views::filter([](auto it) { return it.second != 0; }))
            layout.useful_valves.insert(v.first);
        layout.all_to_all = layout.floyd_warshall();

        return s;
    }
};
}