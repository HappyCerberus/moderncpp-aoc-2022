import day19.robots;

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
    using namespace day19::robots;

    std::stringstream s(R"(Blueprint 1:
  Each ore robot costs 4 ore.
  Each clay robot costs 2 ore.
  Each obsidian robot costs 3 ore and 14 clay.
  Each geode robot costs 2 ore and 7 obsidian.

Blueprint 2:
  Each ore robot costs 2 ore.
  Each clay robot costs 3 ore.
  Each obsidian robot costs 3 ore and 8 clay.
  Each geode robot costs 3 ore and 12 obsidian.)");

    std::vector<Blueprint> blueprints;
    std::ranges::copy(std::views::istream<Blueprint>(s), std::back_inserter(blueprints));
    std::cout << quality_levels(blueprints, 24) << "\n";
    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day19::robots;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    std::vector<Blueprint> blueprints;
    std::ranges::copy(std::views::istream<Blueprint>(file), std::back_inserter(blueprints));
    std::cout << "Sum of quality levels is " << quality_levels(blueprints, 24) << "\n";
    int32_t first = bfs(blueprints[0], 32);
    std::cout << "Blueprint 1 : " << first << "\n";
    int32_t second = bfs(blueprints[1], 32);
    std::cout << "Blueprint 2 : " << second << "\n";
    int32_t third = bfs(blueprints[2], 32);
    std::cout << "Blueprint 3 : " << third << "\n";
    std::cout << "Magic number " << first*second*third << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}