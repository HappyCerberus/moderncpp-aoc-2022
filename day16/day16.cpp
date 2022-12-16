import day16.volcano;

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>

int test() {
    using namespace day16::volcano;
    std::stringstream s(R"(Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve HH has flow rate=22; tunnel leads to valve GG
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II)");

    Layout layout;
    s >> layout;
    std::cout << layout.dfs() << "\n";
    std::cout << layout.dfs_elephant() << "\n";

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day16::volcano;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Layout layout;
    file >> layout;
    std::cout << "Most pressure that can be released is " << layout.dfs() << "\n";
    std::cout << "Most pressure that can be released with an elefant is " << layout.dfs_elephant() << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}