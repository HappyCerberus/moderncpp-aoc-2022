import day24.blizzards;

#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <cassert>
#include <string_view>
#include <fstream>
#include <ranges>
#include <sstream>
#include <list>

int test() {
    using namespace day24::blizzards;
    std::stringstream s(R"(#.######
#>>.<^<#
#.<..<<#
#>v.><>#
#<^v^^>#
######.#)");

    Map m;
    s >> m;

    std::cout << "The minimum number of steps to reach the end is " << search(m, Pos{0,1}, Pos{m.map.size()-1, m.map[0].size()-2}) << "\n";
    std::cout << "The minimum number of steps to also fetch a snack is " << fetch_snack_search(m) << "\n";

    return 0;
}

int parse_and_run(std::string_view path) {
    using namespace day24::blizzards;

    std::fstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << std::quoted(path) << "\n";
        return 1;
    }

    Map m;
    file >> m;
    std::cout << "The minimum number of steps to reach the end is " << search(m, Pos{0,1}, Pos{m.map.size()-1, m.map[0].size()-2}) << "\n";
    std::cout << "The minimum number of steps to also fetch a snack is " << fetch_snack_search(m) << "\n";

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        return test();
    } else if (argc == 2) {
        return parse_and_run(argv[1]);
    }
}